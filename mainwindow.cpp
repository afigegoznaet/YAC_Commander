#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tab.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    init();
    TabbedListView* tab = new TabbedListView(this);
    TabbedListView* rightTab = new TabbedListView(this);
    tab->init();
    rightTab->init();
    tab->setTabOrderIndex(ui->leftTabWidget->addTab(tab, tab->GetDirectory()));
    connect(tab, SIGNAL(dirChanged(QString,int)),ui->leftTabWidget,SLOT(onDirChanged(QString,int)));
    rightTab->setTabOrderIndex(ui->rightTabWidget->addTab(rightTab, rightTab->GetDirectory()));//strictly speaking - this is not needed, since the index will be 0 at this stage
    connect(rightTab, SIGNAL(dirChanged(QString,int)),ui->rightTabWidget,SLOT(onDirChanged(QString,int)));
    tab->setFocus();
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
