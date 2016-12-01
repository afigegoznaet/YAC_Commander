#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    init();
    TabbedListView* leftTab = new TabbedListView(this);
    TabbedListView* rightTab = new TabbedListView(this);
    leftTab->init();
    rightTab->init();
    leftTab->setTabOrderIndex(ui->leftTabWidget->addTab(leftTab, leftTab->GetDirectory()));
    connect(leftTab, SIGNAL(dirChanged(QString,int)),ui->leftTabWidget,SLOT(onDirChanged(QString,int)));
    rightTab->setTabOrderIndex(ui->rightTabWidget->addTab(rightTab, rightTab->GetDirectory()));//strictly speaking - this is not needed, since the index will be 0 at this stage
    connect(rightTab, SIGNAL(dirChanged(QString,int)),ui->rightTabWidget,SLOT(onDirChanged(QString,int)));

    connect(rightTab, SIGNAL(fileMovement(QModelIndexList,FileMovementAction)),this,SLOT(fileMovement(QModelIndexList,FileMovementAction)));
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
        copyFile();
        break;
    case Qt::Key_F6:
        moveFile();
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::copyFile(){
    QStringList moveList;
    QString destination = getDestination();
    QMessageBox msg(QMessageBox::Information,"Movement!", destination, QMessageBox::Ok);
    msg.exec();
}

void MainWindow::moveFile(){
    QStringList moveList;
    QString destination = getDestination();
    QMessageBox msg(QMessageBox::Information,"Movement!", destination, QMessageBox::Ok);
    msg.exec();
}

QString MainWindow::getDestination(){
    auto left = ui->leftTabWidget;
    auto right = ui->rightTabWidget;
    if(left->hasFocus())
        return ((TabbedListView*)(left->currentWidget()))->GetDirectory();
    else
        return ((TabbedListView*)(right->currentWidget()))->GetDirectory();
}

void MainWindow::fileMovement(QItemSelectionModel* model, FileMovementAction action){
    qDebug()<<"Got signal";

    QMessageBox msg(QMessageBox::Information,"Movement!", "Moving something", QMessageBox::Ok);
    msg.exec();
    //qDebug()<<QFileSystemModel::fileInfo(*files.begin());
}
