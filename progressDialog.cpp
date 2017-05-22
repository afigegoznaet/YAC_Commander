#include "progressDialog.h"
//#include <QtTest/QTest>
//#include <unistd.h>


ProgressDialog::ProgressDialog(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	progress(new Ui::ProgressDialog),
	status(1){
	progress->setupUi(this);
	progress->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	progress->tableWidget->setColumnCount(4);
	progress->tableWidget->verticalHeader()->setDefaultSectionSize(this->fontMetrics().height()+6);

	progress->tableWidget->horizontalHeader()->setStretchLastSection(true);
	progress->tableWidget->horizontalHeader()->setSectionsMovable(true);
	progress->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

	QStringList m_TableHeader;
	m_TableHeader<<"#"<<"Name"<<"Text";
	progress->tableWidget->setHorizontalHeaderLabels(m_TableHeader);
	progress->progressBar->setMinimum(0);
	progress->progressBar->setMaximum( 100 );
	connect(this, SIGNAL(dirMoved(int)),this,SLOT(movementResult(int)));
}

ProgressDialog::~ProgressDialog(){
	QSettings settings;
	settings.setValue("ProgressColumns", progress->tableWidget->horizontalHeader()->saveState());
	delete progress;
}

void ProgressDialog::setFileAction(QFileInfoList fileList, QString destination, ACTION action){
	if(!progress->tableWidget->rowCount())
		status = 1;

	int newRow = progress->tableWidget->rowCount();
	switch(action){
	case MOVE:
		foreach (auto fileInfo, fileList) {

			if(!fileInfo.fileName().compare("..", Qt::CaseInsensitive)
					|| !fileInfo.fileName().compare(".", Qt::CaseInsensitive)  )
				continue;


			QString item = "Move "+ fileInfo.fileName() + " to "+destination;
			QString newName = destination+"/"+fileInfo.fileName();

			progress->tableWidget->insertRow( newRow );
			progress->tableWidget->setItem(newRow,0,new QTableWidgetItem("Move"));
			progress->tableWidget->setItem(newRow,1,new QTableWidgetItem(fileInfo.fileName()));
			progress->tableWidget->setItem(newRow,2,new QTableWidgetItem(destination));
			progress->tableWidget->setItem(newRow,3,new QTableWidgetItem(fileInfo.absoluteFilePath()));

		}
		break;
	case COPY:
		foreach (auto fileInfo, fileList) {
			if(!fileInfo.fileName().compare("..", Qt::CaseInsensitive)
					|| !fileInfo.fileName().compare(".", Qt::CaseInsensitive)  )
				continue;

			QString newName = destination+"/"+fileInfo.fileName();
			progress->tableWidget->insertRow( newRow );
			progress->tableWidget->setItem(newRow,0,new QTableWidgetItem("Copy"));
			progress->tableWidget->setItem(newRow,1,new QTableWidgetItem(fileInfo.baseName()));
			progress->tableWidget->setItem(newRow,2,new QTableWidgetItem(destination));
			progress->tableWidget->setItem(newRow,3,new QTableWidgetItem(fileInfo.absoluteFilePath()));

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

void ProgressDialog::onWrite(uint percentsWritten){
	progress->progressBar->setValue(percentsWritten);
}

QMessageBox::StandardButton ProgressDialog::showError(int result){

	status &= (result & 1);

	static const QString errorCopyMasg = "Copying file failed!\nContinue?";
	static const QString errorMoveMasg = "Moving file failed!\nContinue?";
	static const QString errorCopyEOLMasg = "Copying file failed!";
	static const QString errorMoveEOLMasg = "Moving file failed!";
	QMessageBox::StandardButton reply = QMessageBox::Yes;
	switch(result){
		case 10://Move failed
			if(progress->tableWidget->rowCount()>1)
				reply = QMessageBox::question(this, "Error!!!", errorMoveMasg,
											QMessageBox::Yes|QMessageBox::No);
			if(reply == QMessageBox::Yes)
				status = 1;
			else
				QMessageBox::warning(this, "Error!!!", errorMoveEOLMasg);



			break;
		case 0:
			if(progress->tableWidget->rowCount()>1)
				reply = QMessageBox::question(this, "Error!!!", errorCopyMasg,
											QMessageBox::Yes|QMessageBox::No);
			if(reply == QMessageBox::Yes)
				status = 1;
			else
				reply = QMessageBox::question(this, "Error!!!", errorCopyEOLMasg);

			break;
	}
	return reply;
}

void ProgressDialog::movementResult(int result){

	moverBlocker.lock();
	if(progress->tableWidget->rowCount())
		progress->tableWidget->removeRow(0);

	auto reply = showError(result);
	if(reply == QMessageBox::Yes){
		//status = 1;
		QtConcurrent::run(this, &ProgressDialog::DoSomething);
	}
	moverBlocker.unlock();
}


void ProgressDialog::dirMovementResult(int result){
	moverBlocker.lock();
	showError(result);
	moverBlocker.unlock();
}

void ProgressDialog::dirParsing(QDir &dir, QString &action, QString& dest, QList<QString> &createdDirs){

	if(!dir.exists(dest)){
		dir.mkdir(dest);
		createdDirs.append(dest);
	}

	QFileInfoList dirEntries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst);

	foreach (auto file, dirEntries){
		moverBlocker.lock();
		moverBlocker.unlock();
		//this is a very very very bad hack
		if(!status){
			qDebug()<<"Negative status?";
			return;
		}
		QString destination(dest);
		qDebug()<<file.fileName();
		qDebug()<<file.filePath();

		QString source(file.filePath());

		if(file.isDir() ){

			QDir dir(file.filePath());

			destination.append(file.fileName());
			destination.append("/");

			qDebug()<<"***********************";
			qDebug()<<dest;
			qDebug()<<destination;
			qDebug()<<file.absoluteFilePath();


			if( !createdDirs.contains(file.absoluteFilePath().append('/')) )
				dirParsing(dir,action, destination, createdDirs);
			continue;
		}

		destination.append(file.fileName());
		FileMover mover(source, destination, action, this);
		connect(&mover,SIGNAL(bytesProgress(uint)), this, SLOT(onWrite(uint)));
		connect(&mover, SIGNAL(completed(int)),this,SLOT(dirMovementResult(int)));
		connect(this, SIGNAL(setStatus(int)),&mover,SLOT(setStatus(int)), Qt::DirectConnection);
		emit setStatus(status);
		//delete mover;
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
	if (status && progress->tableWidget->rowCount()) {
			//stub.waitForFinished();
		QString source( progress->tableWidget->item(0,3)->text() );
		QString destination( progress->tableWidget->item(0,2)->text() );
		destination.append("/");
		QFileInfo fileInfo(source);
		QString fileName(fileInfo.fileName());
		QString action =  progress->tableWidget->item(0,0)->text();

		if(fileInfo.isDir()){
			QDir dir(source);
			bool movable = isMovable(source,destination);
			destination.append(fileName);
			destination.append("/");
			QList<QString> createdDirs;
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
					dirParsing(dir, action, destination, createdDirs);
					dir.removeRecursively();
				}
			}else
				dirParsing(dir, action, destination,createdDirs);
				dirMoved(1);
			return;
		}


		destination.append(fileName);
		FileMover mover(source, destination, action, this);

		connect(&mover,SIGNAL(bytesProgress(uint)), this, SLOT(onWrite(uint)));
		connect(&mover, SIGNAL(completed(int)),this,SLOT(movementResult(int)), Qt::DirectConnection);
		connect(this, SIGNAL(setStatus(int)),&mover,SLOT(setStatus(int)), Qt::DirectConnection);
		emit setStatus(status);

		//delete mover;

	}else
		emit hideDialogSignal();

}

void ProgressDialog::switchText(){
	progress->pauseButton->setText(pauseButtonLabels[status]);
}

void ProgressDialog::on_pauseButton_clicked(){

	status=!status;
	switchText();
	emit setStatus(status);
	qDebug()<<"*************************************";
	qDebug()<<"status "<<status<<" sent";
}

void ProgressDialog::on_removeButton_clicked(){
	moverBlocker.lock();
	auto items = progress->tableWidget->selectedItems();
	std::set<int> rows;
	foreach (auto item, items)
		rows.insert(item->row());

	foreach (int rowNum, rows) {
		if(0 == rowNum)
			emit setStatus(-1);
		progress->tableWidget->removeRow(rowNum);
	}

	if(!progress->tableWidget->rowCount())
		status = 0;
	moverBlocker.unlock();
}

void ProgressDialog::on_abortButton_clicked(){
	status=0;
	emit setStatus(-1);
	if(progress->tableWidget->rowCount())
		progress->tableWidget->clear();
	qDebug()<<"table widget cleared";
}
