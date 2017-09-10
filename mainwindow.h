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
#include <newdir.h>
#include <utility>
#include "textviewer.h"
#include "dropdown.h"
#include "searchdialog.h"

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
	QString getDirInFocus(bool opposite = false);
	~MainWindow();

public slots:
	void cdTo(const QString&);
	void focusPreviouslyFocused();
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
	SearchDialog* searchDlg;
	QString editor;
	bool leftFocusOut;

	virtual void keyPressEvent(QKeyEvent *event);
	void copyFiles();
	void moveFiles();
	void makeDir();
	void deleteFiles();
	void writeSettings();
	void readSettings();

	QFileInfoList getSelectedFiles();
	TabbedListView* getFocusedTab(void);
	bool getDir(QString &dirName, int numFiles=0, ACTION action=ENUM_TERMINATOR);

};

#endif // MAINWINDOW_H
