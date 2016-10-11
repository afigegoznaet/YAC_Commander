#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tab.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    init();
    Tab* tab = new Tab(this);
    Tab* rightTab = new Tab(tab);
    tab->init();
    rightTab->init();
    ui->leftTabWidget->addTab(tab, tab->GetDirectory());
    ui->rightTabWidget->addTab(rightTab, rightTab->GetDirectory());
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::init(){
	ui->leftTabWidget->setTabsClosable(false);
    ui->leftTabWidget->removeTab(1);
    ui->leftTabWidget->removeTab(0);
	ui->rightTabWidget->setTabsClosable(false);
    ui->rightTabWidget->removeTab(1);
    ui->rightTabWidget->removeTab(0);
	return false;
}
