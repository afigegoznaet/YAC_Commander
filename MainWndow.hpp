#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGlobal>
#include <QMainWindow>
#include "Widgets/FileTabSelector.hpp"
#include "Views/FileTabView.hpp"
#include "ui_mainwindow.h"
#include "ui_progressDialog.h"
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <Dialogs/NewDirDlg.hpp>
#include <utility>
#include "Views/QHexView.hpp"
#include "Widgets/CommandDropdown.hpp"
#include "Dialogs/FileSearchDlg.hpp"
#include "Dialogs/TextViewerDlg.hpp"

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
	FileTableView* focusedTab();
	FileTabSelector* focusedSelector();
	ProgressDialog* getFileMover(){return movementProgress;}
	bool showHidden(){return ui->action_show_hidden_files->isChecked();}

	void parseParams(int argc, char *argv[]);

signals:
	void setFocus(FileTabSelector* tab);
	void setFileAction(QFileInfoList fileList, QString destination,
					   Qt::DropAction action);

public slots:
	void cdTo(const QString&);
	void focusPreviouslyFocused();
	void setFocusSlot(FileTabSelector* tab);
private slots:
	void on_F3_clicked();
	void on_F4_clicked();
	void on_F5_clicked();
	void on_F6_clicked();
	void on_F7_clicked();
	void on_F8_clicked();
	void on_action_show_hidden_files_changed();

private:
	Ui::MainWindow *ui;
	ProgressDialog* movementProgress;
	SearchDialog* searchDlg;
	QString editor;
	bool leftTabHasFocus;

	void copyFiles();
	void moveFiles();
	void makeDir();
	void deleteFiles();
	void writeSettings();
	void readSettings();
	void setupActions();

	QFileInfoList getSelectedFiles();
	void keyPressEvent(QKeyEvent *event) override;
	bool getDir(QString &dirName, int numFiles, Qt::DropAction action);

};

#endif // MAINWINDOW_H
