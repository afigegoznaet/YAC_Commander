#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

//

// static constexpr auto  DEF_EDITOR = "undefined";

#ifdef __linux__
#undef DEF_EDITOR

static constexpr auto DEF_EDITOR = "kate";
#endif
#ifdef _WIN32
#undef DEF_EDITOR

static constexpr auto DEF_EDITOR = "notepad";
#endif
#ifdef __APPLE__
#undef DEF_EDITOR
static constexpr auto DEF_EDITOR = "open";
#endif

class SearchDialog;
class ProgressDialog;

enum FilleOperation : int;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	QString getDirInFocus(bool opposite = false);
	~MainWindow() final;
	FileTableView *	 focusedTab();
	FileTabSelector *focusedSelector();
	ProgressDialog * getFileMover() { return movementProgress; }
	bool			 showHidden();
	void			 parseParams(int argc, char *argv[]);

signals:
	void setFocus(FileTabSelector *tab);
	void setFileAction(QFileInfoList fileList, QString destination,
					   Qt::DropAction action);

public slots:
	void cdTo(const QString &);
	void focusPreviouslyFocused();
	void setFocusSlot(FileTabSelector *tab);
private slots:
	void on_F3_clicked();
	void on_F4_clicked();
	void on_F5_clicked();
	void on_F6_clicked();
	void on_F7_clicked();
	void on_F8_clicked();
	void on_action_show_hidden_files_changed();
	void showAbout();

	void on_actionDefault_editor_triggered();

private:
	Ui::MainWindow *ui;
	ProgressDialog *movementProgress;
	SearchDialog *	searchDlg;
	QString			editor;
	bool			leftTabHasFocus;
	QByteArray		cutActionIndicator;
	QByteArray		cutActionPadding;

	void copyFiles();
	void moveFiles();
	void makeDir();
	void editNewFile();
	void deleteFiles();
	void writeSettings();
	void readSettings();
	void setupActions();
	void cutToClipboard();
	void copyToClipboard();
	void pasteFromClipboard();

	QFileInfoList getSelectedFiles();
	void		  keyPressEvent(QKeyEvent *event) override;
	bool		  getDir(QString &dirName, int numFiles, Qt::DropAction action);
	bool		  getFileName(QString &fileName);
};

#endif // MAINWINDOW_H
