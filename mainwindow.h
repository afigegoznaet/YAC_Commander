#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customtabwidget.h"
#include "tab.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	bool init();

private slots:
    void fileMovement(QModelIndexList files, FileMovementAction action);

private:
	Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
