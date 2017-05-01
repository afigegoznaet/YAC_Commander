#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customtabwidget.h"
#include "progressDialog.h"
#include "tab.h"
#include "ui_mainwindow.h"
#include "ui_progressDialog.h"
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <QLabel>
#include <newdir.h>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots:
	void cdTo(const QString&);
private slots:
	void on_F6_clicked();
	void on_F5_clicked();
	void on_F8_clicked();

	void on_F7_clicked();

private:
	bool init();
	virtual void keyPressEvent(QKeyEvent *event);
	void copyFiles();
	void moveFiles();
	void makeDir();
	void deleteFiles();
	QString getDirInFocus(bool opposite = false);
	QFileInfoList getSelectedFiles();
	Ui::MainWindow *ui;
	ProgressDialog* newDialog;
	void writeSettings();
	void readSettings();
	TabbedListView* getFocusedTab(void);
};

#endif // MAINWINDOW_H
