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
#include <utility>

#define DEF_EDITOR "undefined"

#ifdef __linux__
	#undef DEF_EDITOR
	#define DEF_EDITOR "kate"
#endif
#ifdef _WIN32
	#undef DEF_EDITOR
	#define DEF_EDITOR "notepad"
#endif
#ifdef __APPLE__
	#undef DEF_EDITOR
	#define DEF_EDITOR "TextEdit"
#endif

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
	void on_F3_clicked();
	void on_F4_clicked();
	void on_F5_clicked();
	void on_F6_clicked();
	void on_F7_clicked();
	void on_F8_clicked();

private:

	Ui::MainWindow *ui;
	ProgressDialog* movementProgress;
	QString editor;

	bool init();
	virtual void keyPressEvent(QKeyEvent *event);
	void copyFiles();
	void moveFiles();
	void makeDir();
	void deleteFiles();
	void writeSettings();
	void readSettings();
	QString getDirInFocus(bool opposite = false);
	QFileInfoList getSelectedFiles();
	TabbedListView* getFocusedTab(void);
	bool getDir(QString &dirName);
};

#endif // MAINWINDOW_H
