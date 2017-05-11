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

	QtConcurrent::run(this, &ProgressDialog::DoerSomething);

}

void ProgressDialog::DoerSomething(void){
	while(progressList.size() >= 0 && !this->isHidden() && status){
		DoSomething();
	}

}

void ProgressDialog::onWrite(uint percentsWritten){
	//qDebug()<<"GOT ON WRITE~!!!!!!!!!!!!!!!!!!1111";
	//qDebug()<<percentsWritten;
	//Fucking stupid bullshit QT doesn't allow to update UI on non main thread!!!!
	progress->progressBar->setValue(percentsWritten);
	//qDebug()<<"Not here";
}

void ProgressDialog::movementResult(bool result){
	status &= result;
	if(!result){
		QString errorMasg;
		if(progress->tableWidget->item(0,0)->text().compare("Copy",Qt::CaseInsensitive))
			//QMessageBox::warning(this, "Error!", "Copying file failed!");
			errorMasg="Copying file failed!\nContinue?";
		else
			errorMasg="Moving file failed!\nContinue?";

		auto reply = QMessageBox::question(this, "Error!!!", errorMasg,
										QMessageBox::Yes|QMessageBox::No);
		if(reply == QMessageBox::Yes){
			status = 1;
			QtConcurrent::run(this, &ProgressDialog::DoerSomething);
		}
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
		mover->status = connect(mover, SIGNAL(completed(bool)),this,SLOT(movementResult(bool)));
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


void ProgressDialog::DoSomething(){

	if (progressList.size()) {
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
		mover->status = connect(mover, SIGNAL(completed(bool)),this,SLOT(movementResult(bool)));

		delete mover;
		progress->tableWidget->removeRow(0);
		progressList.pop_front();

	}else
		emit hideDialogSignal();

}
