#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customtabwidget.h"
#include "dialog.h"
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

private:
	bool init();
	virtual void keyPressEvent(QKeyEvent *event);
	void copyFiles();
	void moveFiles();
	void deleteFiles();
	QString getDestination();
	QFileInfoList getSelectedFiles();
	Ui::MainWindow *ui;
	Dialog newDialog;
	void writeSettings();
	void readSettings();
};

#endif // MAINWINDOW_H
