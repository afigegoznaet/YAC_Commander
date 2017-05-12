#include "progressDialog.h"
//#include <QtTest/QTest>
//#include <unistd.h>


ProgressDialog::ProgressDialog(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	progress(new Ui::ProgressDialog),
	status(1){
	progress->setupUi(this);
	progress->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	progress->tableWidget->setColumnCount(3);
	progress->tableWidget->verticalHeader()->setDefaultSectionSize(this->fontMetrics().height()+6);

	progress->tableWidget->horizontalHeader()->setStretchLastSection(true);
	progress->tableWidget->horizontalHeader()->setSectionsMovable(true);
	progress->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

	QStringList m_TableHeader;
	m_TableHeader<<"#"<<"Name"<<"Text";
	progress->tableWidget->setHorizontalHeaderLabels(m_TableHeader);
	progress->progressBar->setMinimum(0);
	progress->progressBar->setMaximum( 100 );

}

ProgressDialog::~ProgressDialog(){
	QSettings settings;
	settings.setValue("ProgressColumns", progress->tableWidget->horizontalHeader()->saveState());
	delete progress;
}

void ProgressDialog::setFileAction(QFileInfoList fileList, QString destination, ACTION action){
	int newRow = progress->tableWidget->rowCount();

	switch(action){
	case MOVE:
		foreach (auto fileInfo, fileList) {
			QString item = "Move "+ fileInfo.fileName() + " to "+destination;
			QString newName = destination+"/"+fileInfo.fileName();
			//QListWidgetItem item;
			//item.setText(fileInfo.fileName());


			progress->tableWidget->insertRow( newRow );
			progress->tableWidget->setItem(newRow,0,new QTableWidgetItem("Move"));
			progress->tableWidget->setItem(newRow,1,new QTableWidgetItem(fileInfo.fileName()));
			progress->tableWidget->setItem(newRow,2,new QTableWidgetItem(destination));

			progressList.append(fileInfo);
			//newDialog.setFileAction(fileList, destination);
			//QFile::rename(fileInfo.absoluteFilePath(), newName);
		}
		break;
	case COPY:
		foreach (auto fileInfo, fileList) {
			QString newName = destination+"/"+fileInfo.fileName();
			//progress->tableWidget->addItem(fileInfo.fileName());
			progress->tableWidget->insertRow( newRow );
			progress->tableWidget->setItem(newRow,0,new QTableWidgetItem("Copy"));
			progress->tableWidget->setItem(newRow,1,new QTableWidgetItem(fileInfo.baseName()));
			progress->tableWidget->setItem(newRow,2,new QTableWidgetItem(destination));
			progressList.append(fileInfo);
		}
		break;
	case LN:
	case LN_S:
		break;
	case ENUM_TERMINATOR:
		QMessageBox::warning(this, "Error!", "Unsupported action!");
		break;
	}


	connect(this,  SIGNAL(sendErrMsg(QString )), this,SLOT(errorMsg(QString )), Qt::QueuedConnection);
	connect(this, SIGNAL(hideDialogSignal()), this,SLOT(hideDialogSlot()), Qt::QueuedConnection);

	QtConcurrent::run(this, &ProgressDialog::DoSomething);

}
/*
void ProgressDialog::DoerSomething(void){
	//while(progressList.size() >= 0 && !this->isHidden() && status){
		DoSomething();
	//}

}*/

void ProgressDialog::onWrite(uint percentsWritten){
	//qDebug()<<"GOT ON WRITE~!!!!!!!!!!!!!!!!!!1111";
	//qDebug()<<percentsWritten;
	//Fucking stupid bullshit QT doesn't allow to update UI on non main thread!!!!
	progress->progressBar->setValue(percentsWritten);
	//qDebug()<<"Not here";
}

void ProgressDialog::movementResult(int result){
	status &= (result & 1);
	static const QString errorCopyMasg = "Copying file failed!\nContinue?";
	static const QString errorMoveMasg = "Moving file failed!\nContinue?";
	static const QString errorCopyEOLMasg = "Copying file failed!";
	static const QString errorMoveEOLMasg = "Moving file failed!";
	QMessageBox::StandardButton reply = QMessageBox::Yes;
	switch(result){
		case 10://Move failed
			if(progressList.size()>1)
				reply = QMessageBox::question(this, "Error!!!", errorMoveMasg,
											QMessageBox::Yes|QMessageBox::No);
			else
				QMessageBox::warning(this, "Error!!!", errorMoveEOLMasg);



			break;
		case 0:
			if(progressList.size()>1)
				reply = QMessageBox::question(this, "Error!!!", errorCopyMasg,
											QMessageBox::Yes|QMessageBox::No);
			else
				reply = QMessageBox::question(this, "Error!!!", errorCopyEOLMasg);

			break;
	}


	cond.wakeOne();

	QMutex blocker;
	blocker.lock();
	condStatus.wait(&blocker);//this releases the mutex

	if(reply == QMessageBox::Yes){
		status = 1;
		QtConcurrent::run(this, &ProgressDialog::DoSomething);
	}
}


void ProgressDialog::dirParsing(QDir &dir, QString &action, QString& dest){

	if(!dir.exists(dest))
		dir.mkdir(dest);

	QFileInfoList dirEntries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst);

	foreach (auto file, dirEntries){

		QString destination(dest);
		qDebug()<<file.fileName();
		qDebug()<<file.filePath();

		QString source(file.filePath());

		if(file.isDir() ){

			QDir dir(file.filePath());

			destination.append(file.fileName());
			destination.append("/");

			if( dest.compare(file.absoluteFilePath().append('/')) )
				dirParsing(dir,action, destination);
			continue;
		}

		destination.append(file.fileName());
		FileMover* mover = new FileMover(source, destination, action, this);
		mover->progress = connect(mover,SIGNAL(bytesProgress(uint)), this, SLOT(onWrite(uint)));
		mover->status = connect(mover, SIGNAL(completed(int)),this,SLOT(movementResult(int)));
		delete mover;
		//mover should be auto deleted now
	}
};

void ProgressDialog::errorMsg(QString errorText){
	QMessageBox::warning(this, "Error",errorText);
	cond.wakeOne();
}
void ProgressDialog::hideDialogSlot(){
	this->hide();
}


void ProgressDialog::DoSomething(void){
	if (!progressList.isEmpty()) {
			//stub.waitForFinished();
		QString source(progressList.front().filePath());
		QString destination( progress->tableWidget->item(0,2)->text() );
		destination.append("/");
		QFileInfo fileInfo(source);
		QString fileName(fileInfo.fileName());

		if(!fileName.compare("..", Qt::CaseInsensitive) || !fileName.compare(".", Qt::CaseInsensitive)  ){

			progress->tableWidget->removeRow(0);
			progressList.pop_front();
			return;
		}

		QString action =  progress->tableWidget->item(0,0)->text();

		if(fileInfo.isDir()){


			QDir dir(fileInfo.filePath());

			bool movable = isMovable(source,destination);
			destination.append(fileName);
			destination.append("/");
			/**
			  Too many "if"s, gotta do something about it
			  */
			if(!action.compare("Move", Qt::CaseInsensitive)){
				if(movable){
					if(destination.startsWith(source)){
						emit sendErrMsg("Can not move directory into itself");
						QMutex blocker;
						blocker.lock();
						cond.wait(&blocker);//this releases the mutex
						//blocker.unlock();
					}
					else
						dir.rename(source,destination);
				}else{
					dirParsing(dir, action, destination);
					dir.removeRecursively();
				}
			}else
				dirParsing(dir, action, destination);


			progress->tableWidget->removeRow(0);
			progressList.pop_front();
			return;
		}


		destination.append(fileName);
		FileMover* mover = new FileMover(source, destination, action, this);

		mover->progress = connect(mover,SIGNAL(bytesProgress(uint)), this, SLOT(onWrite(uint)));
		mover->status = connect(mover, SIGNAL(completed(int)),this,SLOT(movementResult(int)));

		delete mover;

		QMutex blocker;
		blocker.lock();
		cond.wait(&blocker);//this releases the mutex

		progress->tableWidget->removeRow(0);
		progressList.pop_front();

		condStatus.wakeOne();

	}else
		emit hideDialogSignal();

}
