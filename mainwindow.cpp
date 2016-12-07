#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow), newDialog(this)
{
	ui->setupUi(this);
	init();
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
	leftTab->setFocus();
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::init(){
	ui->leftTabWidget->setTabsClosable(false);
	//ui->leftTabWidget->removeTab(0);
	ui->rightTabWidget->setTabsClosable(false);
	//ui->rightTabWidget->removeTab(0);
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
	QFileInfoList fileList = getSelectedFiles();
	QString destination = getDestination();
	QString message = "Copy " + QString::number(fileList.size()) + " files to \n"+destination +" ?";
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Moving ", message,
									QMessageBox::Yes|QMessageBox::No);

	if(reply == QMessageBox::No)
		return;

	foreach (auto fileInfo, fileList) {
		QString newName = destination+"/"+fileInfo.fileName();
		QFile::copy(fileInfo.absoluteFilePath(), newName);
	}
	QMessageBox msg(QMessageBox::Information,"Copying!", destination, QMessageBox::Ok);
	msg.exec();
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
	newDialog.show();
	//foreach (auto fileInfo, fileList) {
		//QString newName = destination+"/"+fileInfo.fileName();
		newDialog.setFileAction(fileList, destination, MOVE);
		//QFile::rename(fileInfo.absoluteFilePath(), newName);
   // }

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
	auto left = (TabbedListView*) ui->leftTabWidget->currentWidget();
	auto right = (TabbedListView*) ui->rightTabWidget->currentWidget();

	if(left->hasFocus())
		return left->getSelectedFiles();
	else
		return right->getSelectedFiles();
}
