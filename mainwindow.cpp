#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <windows.h>
#include "ui_progressDialog.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow){
	ui->setupUi(this);
	init();
	readSettings();
	TabbedListView* leftTab = new TabbedListView(this);
	TabbedListView* rightTab = new TabbedListView(this);
	leftTab->init();
	rightTab->init();
	leftTab->setTabOrderIndex(ui->leftTabWidget->addTab(leftTab, leftTab->GetDirectory()));
	connect(leftTab,            SIGNAL(dirChanged(QString,int)),
			ui->leftTabWidget,  SLOT(onDirChanged(QString,int))
			);
	connect(leftTab,            SIGNAL(focusEvent(bool)),
			ui->leftTabWidget,  SLOT(onFocusEvent(bool))
			);
	rightTab->setTabOrderIndex(ui->rightTabWidget->addTab(rightTab, rightTab->GetDirectory()));//strictly speaking - this is not needed, since the index will be 0 at this stage
	connect(rightTab,           SIGNAL(dirChanged(QString,int)),
			ui->rightTabWidget, SLOT(onDirChanged(QString,int))
			);
	connect(rightTab,           SIGNAL(focusEvent(bool)),
			ui->rightTabWidget, SLOT(onFocusEvent(bool))
			);

	connect(ui->quickBar,SIGNAL(cdTo(QString)), this, SLOT(cdTo(QString)));
	auto defaultState = leftTab->horizontalHeader()->saveState();//header state
	QSettings settings;
	auto headerState = settings.value("Columns", defaultState).toByteArray();
	leftTab->horizontalHeader()->restoreState(headerState);
	rightTab->horizontalHeader()->restoreState(headerState);

	headerState = settings.value("ProgressColumns", defaultState).toByteArray();
	newDialog->progress->tableWidget->horizontalHeader()->restoreState(headerState);



	leftTab->setFocus();
}

MainWindow::~MainWindow(){
	writeSettings();
	delete ui;
}

void MainWindow::writeSettings(){
	QSettings settings;

	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();
}

void MainWindow::readSettings(){
	QSettings settings;

	settings.beginGroup("MainWindow");
	resize(settings.value("size", QSize(400, 400)).toSize());
	move(settings.value("pos", QPoint(200, 200)).toPoint());
	settings.endGroup();
}

bool MainWindow::init(){
	ui->leftTabWidget->setTabsClosable(false);
	//ui->leftTabWidget->removeTab(0);
	ui->rightTabWidget->setTabsClosable(false);
	//ui->rightTabWidget->removeTab(0);
	newDialog = new ProgressDialog(this);
	return false;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
	auto key = event->key();
	qDebug() << "Got event!!!!!!!!!!!!!!!!!1";
	switch (key) {
	case Qt::Key_F5:
		copyFiles();

		break;
	case Qt::Key_F6:
		moveFiles();
		break;
	case Qt::Key_F8:
		deleteFiles();
		break;
	default:
		QMainWindow::keyPressEvent(event);
		break;
	case Qt::Key_Tab:
		if(ui->leftTabWidget->currentWidget()->hasFocus())
			ui->rightTabWidget->setFocus();
		else
			ui->leftTabWidget->setFocus();

		break;
	}
}

void MainWindow::copyFiles(){

	qDebug()<<"Main Window"<<thread();
	qDebug()<<"\n\n\n\n*****\n\n\n";

	QFileInfoList fileList = getSelectedFiles();
	QString destination = getDestination();
	QString message = "Copy " + QString::number(fileList.size()) + " files to \n"+destination +" ?";
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Moving ", message,
									QMessageBox::Yes|QMessageBox::No);

	if(reply == QMessageBox::No)
		return;
	newDialog->show();
	newDialog->setFileAction(fileList, destination, COPY);
	/*foreach (auto fileInfo, fileList) {
		QString newName = destination+"/"+fileInfo.fileName();
		//QFile::copy(fileInfo.absoluteFilePath(), newName);
	}
	QMessageBox msg(QMessageBox::Information,"Copying!", destination, QMessageBox::Ok);
	msg.exec();*/
}

void MainWindow::moveFiles(){
	QFileInfoList fileList = getSelectedFiles();
	QString destination = getDestination();

	QString message = "Move " + QString::number(fileList.size()) + " files to \n"+destination;

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Moving ", message,
									QMessageBox::Yes|QMessageBox::No);

	if(reply == QMessageBox::No)
		return;
	newDialog->show();
	//foreach (auto fileInfo, fileList) {
		//QString newName = destination+"/"+fileInfo.fileName();
		newDialog->setFileAction(fileList, destination, MOVE);
		//QFile::rename(fileInfo.absoluteFilePath(), newName);
   // }
}

void MainWindow::deleteFiles(){
	QFileInfoList fileList = getSelectedFiles();

	QString message = "Delete " + QString::number(fileList.size()) + " files?\n";

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Deleting ", message,
									QMessageBox::Yes|QMessageBox::No);

	if(reply == QMessageBox::No)
		return;
	//newDialog->show();
	foreach (auto fileInfo, fileList) {
		//QString newName = destination+"/"+fileInfo.fileName();
		//newDialog->setFileAction(fileList, destination, MOVE);
		QFile::remove( fileInfo.absoluteFilePath());
	}
}

QString MainWindow::getDestination(){
	auto left = (TabbedListView*) ui->leftTabWidget->currentWidget();
	auto right = (TabbedListView*) ui->rightTabWidget->currentWidget();


	if(left->hasFocus())
		return right->GetDirectory();
	else
		return left->GetDirectory();
}

QFileInfoList MainWindow::getSelectedFiles(){
	return getFocusedTab()->getSelectedFiles();
}

void MainWindow::on_F5_clicked(){
	copyFiles();
}

void MainWindow::on_F6_clicked(){
	moveFiles();
}

void MainWindow::on_F8_clicked(){
	deleteFiles();
}

TabbedListView* MainWindow::getFocusedTab(void){
	auto left = (TabbedListView*) ui->leftTabWidget->currentWidget();
	auto right = (TabbedListView*) ui->rightTabWidget->currentWidget();


	if(left->hasFocus())
		return left;
	return right;
}

void MainWindow::cdTo(const QString &dir){

	qDebug()<<"Got it!!!!";
	getFocusedTab()->cdTo(dir);
}
