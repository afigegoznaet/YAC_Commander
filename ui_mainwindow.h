/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include "customtabwidget.h"
#include "customtoolbar.h"
#include "dropdown.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionDisk;
    QAction *actionRefresh;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *buttons;
    QPushButton *F3;
    QPushButton *F4;
    QPushButton *F5;
    QPushButton *F6;
    QPushButton *F7;
    QPushButton *F8;
    DropDown *commandsBox;
    QHBoxLayout *horizontalLayout;
    CustomTabWidget *leftTabWidget;
    CustomTabWidget *rightTabWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *rightLabel;
    QLabel *leftLabel;
    QMenuBar *menubar;
    QMenu *menuFiles;
    QMenu *menuCommand;
    QMenu *menuCommands;
    QMenu *menuNet;
    QMenu *menuShow;
    QMenu *menuConfiguration;
    QMenu *menuStart;
    QStatusBar *statusbar;
    CustomToolbar *quickBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionDisk = new QAction(MainWindow);
        actionDisk->setObjectName(QString::fromUtf8("actionDisk"));
        actionRefresh = new QAction(MainWindow);
        actionRefresh->setObjectName(QString::fromUtf8("actionRefresh"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        buttons = new QHBoxLayout();
        buttons->setObjectName(QString::fromUtf8("buttons"));
        F3 = new QPushButton(centralwidget);
        F3->setObjectName(QString::fromUtf8("F3"));
        F3->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F3);

        F4 = new QPushButton(centralwidget);
        F4->setObjectName(QString::fromUtf8("F4"));
        F4->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F4);

        F5 = new QPushButton(centralwidget);
        F5->setObjectName(QString::fromUtf8("F5"));
        F5->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F5);

        F6 = new QPushButton(centralwidget);
        F6->setObjectName(QString::fromUtf8("F6"));
        F6->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F6);

        F7 = new QPushButton(centralwidget);
        F7->setObjectName(QString::fromUtf8("F7"));
        F7->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F7);

        F8 = new QPushButton(centralwidget);
        F8->setObjectName(QString::fromUtf8("F8"));
        F8->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F8);


        gridLayout->addLayout(buttons, 3, 0, 1, 1);

        commandsBox = new DropDown(centralwidget);
        commandsBox->setObjectName(QString::fromUtf8("commandsBox"));
        commandsBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(commandsBox, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        leftTabWidget = new CustomTabWidget(centralwidget);
        leftTabWidget->setObjectName(QString::fromUtf8("leftTabWidget"));
        leftTabWidget->setFocusPolicy(Qt::NoFocus);
        leftTabWidget->setAcceptDrops(true);
        leftTabWidget->setTabsClosable(true);
        leftTabWidget->setMovable(true);

        horizontalLayout->addWidget(leftTabWidget);

        rightTabWidget = new CustomTabWidget(centralwidget);
        rightTabWidget->setObjectName(QString::fromUtf8("rightTabWidget"));
        rightTabWidget->setFocusPolicy(Qt::NoFocus);
        rightTabWidget->setAcceptDrops(true);
        rightTabWidget->setTabShape(QTabWidget::Rounded);
        rightTabWidget->setDocumentMode(false);
        rightTabWidget->setTabsClosable(true);
        rightTabWidget->setMovable(true);

        horizontalLayout->addWidget(rightTabWidget);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        rightLabel = new QLabel(centralwidget);
        rightLabel->setObjectName(QString::fromUtf8("rightLabel"));

        horizontalLayout_3->addWidget(rightLabel);

        leftLabel = new QLabel(centralwidget);
        leftLabel->setObjectName(QString::fromUtf8("leftLabel"));

        horizontalLayout_3->addWidget(leftLabel);


        gridLayout->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        commandsBox->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 19));
        menuFiles = new QMenu(menubar);
        menuFiles->setObjectName(QString::fromUtf8("menuFiles"));
        menuCommand = new QMenu(menubar);
        menuCommand->setObjectName(QString::fromUtf8("menuCommand"));
        menuCommands = new QMenu(menubar);
        menuCommands->setObjectName(QString::fromUtf8("menuCommands"));
        menuNet = new QMenu(menubar);
        menuNet->setObjectName(QString::fromUtf8("menuNet"));
        menuShow = new QMenu(menubar);
        menuShow->setObjectName(QString::fromUtf8("menuShow"));
        menuConfiguration = new QMenu(menubar);
        menuConfiguration->setObjectName(QString::fromUtf8("menuConfiguration"));
        menuStart = new QMenu(menubar);
        menuStart->setObjectName(QString::fromUtf8("menuStart"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        quickBar = new CustomToolbar(MainWindow);
        quickBar->setObjectName(QString::fromUtf8("quickBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(quickBar->sizePolicy().hasHeightForWidth());
        quickBar->setSizePolicy(sizePolicy1);
        quickBar->setMovable(false);
        quickBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, quickBar);

        menubar->addAction(menuFiles->menuAction());
        menubar->addAction(menuCommand->menuAction());
        menubar->addAction(menuCommands->menuAction());
        menubar->addAction(menuNet->menuAction());
        menubar->addAction(menuShow->menuAction());
        menubar->addAction(menuConfiguration->menuAction());
        menubar->addAction(menuStart->menuAction());
        quickBar->addSeparator();

        retranslateUi(MainWindow);

        leftTabWidget->setCurrentIndex(-1);
        rightTabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
		MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
		actionDisk->setText(QApplication::translate("MainWindow", "Disk", 0));
#ifndef QT_NO_TOOLTIP
		actionDisk->setToolTip(QApplication::translate("MainWindow", "Disk", 0));
#endif // QT_NO_TOOLTIP
		actionRefresh->setText(QApplication::translate("MainWindow", "Refresh", 0));
#ifndef QT_NO_TOOLTIP
		actionRefresh->setToolTip(QApplication::translate("MainWindow", "Refresh", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
		F3->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#00aa00;\">View file in text mode</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
		F3->setText(QApplication::translate("MainWindow", "F3 View", 0));
#ifndef QT_NO_TOOLTIP
		F4->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ff5500;\">Edit file in %s</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
		F4->setText(QApplication::translate("MainWindow", "F4 Edit", 0));
#ifndef QT_NO_TOOLTIP
		F5->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#00aa00;\">Copy selected items to whatever folder is opened in the opposite tab</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
		F5->setText(QApplication::translate("MainWindow", "F5 Copy", 0));
#ifndef QT_NO_TOOLTIP
		F6->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#00aa00;\">Move selected items to whatever folder is opened in the opposite tab</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
		F6->setText(QApplication::translate("MainWindow", "F6 Move", 0));
#ifndef QT_NO_TOOLTIP
		F7->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#00aa00;\">Create a new folder</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
		F7->setText(QApplication::translate("MainWindow", "F7 New Folder", 0));
#ifndef QT_NO_TOOLTIP
		F8->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ff0000;\">Delete selected items</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
		F8->setText(QApplication::translate("MainWindow", "F8 Delete", 0));
#ifndef QT_NO_ACCESSIBILITY
		leftTabWidget->setAccessibleDescription(QApplication::translate("MainWindow", "Left", 0));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
		rightTabWidget->setAccessibleDescription(QApplication::translate("MainWindow", "Right", 0));
#endif // QT_NO_ACCESSIBILITY
		rightLabel->setText(QApplication::translate("MainWindow", "TextLabel", 0));
		leftLabel->setText(QApplication::translate("MainWindow", "TextLabel", 0));
		menuFiles->setTitle(QApplication::translate("MainWindow", "Files", 0));
		menuCommand->setTitle(QApplication::translate("MainWindow", "Mark", 0));
		menuCommands->setTitle(QApplication::translate("MainWindow", "Commands", 0));
		menuNet->setTitle(QApplication::translate("MainWindow", "Net", 0));
		menuShow->setTitle(QApplication::translate("MainWindow", "Show", 0));
		menuConfiguration->setTitle(QApplication::translate("MainWindow", "Configuration", 0));
		menuStart->setTitle(QApplication::translate("MainWindow", "Start", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
