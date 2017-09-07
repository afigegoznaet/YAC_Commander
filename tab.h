
#ifndef TAB_H
#define TAB_H

#include <QWidget>
//#include <QListView>
#include <QTableView>
#include <QHeaderView>
#include <QFileSystemModel>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QItemSelectionModel>
#include <QSettings>
#include <QInputDialog>
#include "orderedfilesystemmodel.h"

class TabbedListView : public QTableView
{
	Q_OBJECT

	enum Action{PLUS, MINUS, ASTERISK};
public:
	explicit TabbedListView(QDir directory, QWidget *parent = 0);
	TabbedListView(QWidget *parent) : TabbedListView(QDir::homePath(),parent){
	};
	QString GetDirectory(){

		return model->rootPath();
	};
	void init();
	void setTabOrderIndex(int index){
		this->index=index;
	};
	QFileInfoList getSelectedFiles();
	void cdTo(const QString&);

signals:
	//void activated(const QModelIndex &index);
	void dirChanged(const QString dirName, int index);
	void focusEvent(bool);
	//void fileMovement(QItemSelectionModel* model, FileMovementAction action);

public slots:
	void on_doubleClicked(const QModelIndex &index);
	void setCurrentSelection(QString);
	void headerClicked(int section);
	void rowsRemoved(const QModelIndex &parent, int first, int);
	void rowsInserted(const QModelIndex &parent, int first, int);
protected:
	virtual void keyPressEvent(QKeyEvent * event);
	void chDir(const QModelIndex &index, bool in_out);
	virtual void focusInEvent(QFocusEvent* event);
	virtual void focusOutEvent(QFocusEvent* event);


private:
	QString directory;
	//QString *selection;
	OrderedFileSystemModel* model;
	//QFileSystemModel* model;
	int index;
	QTabWidget* metaTab;
	virtual void mousePressEvent(QMouseEvent *event);
	void queryDialog(QString& filter, Action act);
	void setSelection(Action act);
};

#endif // TAB_H

