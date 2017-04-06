#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customtabwidget.h"
#include "progressDialog.h"
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

private slots:
	void on_F6_clicked();
	void on_F5_clicked();

	void on_F8_clicked();

private:
	bool init();
	virtual void keyPressEvent(QKeyEvent *event);
	void copyFiles();
	void moveFiles();
	void deleteFiles();
	QString getDestination();
	QFileInfoList getSelectedFiles();
	Ui::MainWindow *ui;
	ProgressDialog* newDialog;
	void writeSettings();
	void readSettings();
};

#endif // MAINWINDOW_H
