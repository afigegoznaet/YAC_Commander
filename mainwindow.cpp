#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
		init();
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::init(){
	ui->leftTabWidget->setTabsClosable(false);
	ui->leftTabWidget->removeTab(1);
	ui->rightTabWidget->setTabsClosable(false);
	ui->rightTabWidget->removeTab(1);
	return false;
}
