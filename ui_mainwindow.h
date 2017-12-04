/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "FileTabSelector.h"
#include "QuickAccessBar.h"
#include "CommandDropdown.h"

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
    CommandDropDown *commandsBox;
    QHBoxLayout *tabViewLayout;
    FileTabSelector *leftTabWidget;
    FileTabSelector *rightTabWidget;
    QHBoxLayout *labelLayout;
    QLabel *leftLabel;
    QLabel *rightLabel;
    QMenuBar *menubar;
    QMenu *menuFiles;
    QMenu *menuCommand;
    QMenu *menuCommands;
    QMenu *menuNet;
    QMenu *menuShow;
    QMenu *menuConfiguration;
    QMenu *menuStart;
    QStatusBar *statusbar;
    QuickAccessBar *quickBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionDisk = new QAction(MainWindow);
        actionDisk->setObjectName(QStringLiteral("actionDisk"));
        actionRefresh = new QAction(MainWindow);
        actionRefresh->setObjectName(QStringLiteral("actionRefresh"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        buttons = new QHBoxLayout();
        buttons->setObjectName(QStringLiteral("buttons"));
        F3 = new QPushButton(centralwidget);
        F3->setObjectName(QStringLiteral("F3"));
        F3->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F3);

        F4 = new QPushButton(centralwidget);
        F4->setObjectName(QStringLiteral("F4"));
        F4->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F4);

        F5 = new QPushButton(centralwidget);
        F5->setObjectName(QStringLiteral("F5"));
        F5->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F5);

        F6 = new QPushButton(centralwidget);
        F6->setObjectName(QStringLiteral("F6"));
        F6->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F6);

        F7 = new QPushButton(centralwidget);
        F7->setObjectName(QStringLiteral("F7"));
        F7->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F7);

        F8 = new QPushButton(centralwidget);
        F8->setObjectName(QStringLiteral("F8"));
        F8->setFocusPolicy(Qt::NoFocus);

        buttons->addWidget(F8);


        gridLayout->addLayout(buttons, 3, 0, 1, 1);

        commandsBox = new CommandDropDown(centralwidget);
        commandsBox->setObjectName(QStringLiteral("commandsBox"));
        commandsBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(commandsBox, 2, 0, 1, 1);

        tabViewLayout = new QHBoxLayout();
        tabViewLayout->setObjectName(QStringLiteral("tabViewLayout"));
        leftTabWidget = new FileTabSelector(centralwidget);
        leftTabWidget->setObjectName(QStringLiteral("leftTabWidget"));
        leftTabWidget->setFocusPolicy(Qt::NoFocus);
        leftTabWidget->setAcceptDrops(true);
        leftTabWidget->setTabsClosable(true);
        leftTabWidget->setMovable(true);

        tabViewLayout->addWidget(leftTabWidget);

        rightTabWidget = new FileTabSelector(centralwidget);
        rightTabWidget->setObjectName(QStringLiteral("rightTabWidget"));
        rightTabWidget->setFocusPolicy(Qt::NoFocus);
        rightTabWidget->setAcceptDrops(true);
        rightTabWidget->setTabShape(QTabWidget::Rounded);
        rightTabWidget->setDocumentMode(false);
        rightTabWidget->setTabsClosable(true);
        rightTabWidget->setMovable(true);

        tabViewLayout->addWidget(rightTabWidget);


        gridLayout->addLayout(tabViewLayout, 1, 0, 1, 1);

        labelLayout = new QHBoxLayout();
        labelLayout->setObjectName(QStringLiteral("labelLayout"));
        leftLabel = new QLabel(centralwidget);
        leftLabel->setObjectName(QStringLiteral("leftLabel"));

        labelLayout->addWidget(leftLabel);

        rightLabel = new QLabel(centralwidget);
        rightLabel->setObjectName(QStringLiteral("rightLabel"));

        labelLayout->addWidget(rightLabel);


        gridLayout->addLayout(labelLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        commandsBox->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 19));
        menuFiles = new QMenu(menubar);
        menuFiles->setObjectName(QStringLiteral("menuFiles"));
        menuCommand = new QMenu(menubar);
        menuCommand->setObjectName(QStringLiteral("menuCommand"));
        menuCommands = new QMenu(menubar);
        menuCommands->setObjectName(QStringLiteral("menuCommands"));
        menuNet = new QMenu(menubar);
        menuNet->setObjectName(QStringLiteral("menuNet"));
        menuShow = new QMenu(menubar);
        menuShow->setObjectName(QStringLiteral("menuShow"));
        menuConfiguration = new QMenu(menubar);
        menuConfiguration->setObjectName(QStringLiteral("menuConfiguration"));
        menuStart = new QMenu(menubar);
        menuStart->setObjectName(QStringLiteral("menuStart"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        quickBar = new QuickAccessBar(MainWindow);
        quickBar->setObjectName(QStringLiteral("quickBar"));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionDisk->setText(QApplication::translate("MainWindow", "Disk", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionDisk->setToolTip(QApplication::translate("MainWindow", "Disk", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionRefresh->setText(QApplication::translate("MainWindow", "Refresh", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionRefresh->setToolTip(QApplication::translate("MainWindow", "Refresh", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        F3->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#00aa00;\">View file in text mode</span></p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        F3->setText(QApplication::translate("MainWindow", "F3 View", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        F4->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ff5500;\">Edit file in %s</span></p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        F4->setText(QApplication::translate("MainWindow", "F4 Edit", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        F5->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#00aa00;\">Copy selected items to whatever folder is opened in the opposite tab</span></p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        F5->setText(QApplication::translate("MainWindow", "F5 Copy", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        F6->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#00aa00;\">Move selected items to whatever folder is opened in the opposite tab</span></p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        F6->setText(QApplication::translate("MainWindow", "F6 Move", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        F7->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#00aa00;\">Create a new folder</span></p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        F7->setText(QApplication::translate("MainWindow", "F7 New Folder", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        F8->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ff0000;\">Delete selected items</span></p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        F8->setText(QApplication::translate("MainWindow", "F8 Delete", Q_NULLPTR));
#ifndef QT_NO_ACCESSIBILITY
        leftTabWidget->setAccessibleDescription(QApplication::translate("MainWindow", "Left", Q_NULLPTR));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        rightTabWidget->setAccessibleDescription(QApplication::translate("MainWindow", "Right", Q_NULLPTR));
#endif // QT_NO_ACCESSIBILITY
        leftLabel->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        rightLabel->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        menuFiles->setTitle(QApplication::translate("MainWindow", "Files", Q_NULLPTR));
        menuCommand->setTitle(QApplication::translate("MainWindow", "Mark", Q_NULLPTR));
        menuCommands->setTitle(QApplication::translate("MainWindow", "Commands", Q_NULLPTR));
        menuNet->setTitle(QApplication::translate("MainWindow", "Net", Q_NULLPTR));
        menuShow->setTitle(QApplication::translate("MainWindow", "Show", Q_NULLPTR));
        menuConfiguration->setTitle(QApplication::translate("MainWindow", "Configuration", Q_NULLPTR));
        menuStart->setTitle(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
