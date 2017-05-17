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

class TabbedListView : public QTableView
{
	Q_OBJECT

public:
	explicit TabbedListView(QDir directory, QWidget *parent = 0);
	~TabbedListView();
	TabbedListView(QWidget *parent) : TabbedListView(QDir::homePath(),parent){
	};
	QString GetDirectory(){
		return model->rootDirectory().absolutePath();
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
	void setCurrentSelection(QString sel);
	void headerClicked(int section);
protected:
	virtual void keyPressEvent(QKeyEvent * event);
	void chDir(const QModelIndex &index, bool in_out);
	virtual void focusInEvent(QFocusEvent* event);
	virtual void focusOutEvent(QFocusEvent* event);


private:
	QString directory;
	//QString *selection;
	QFileSystemModel* model;
	int index;
	QTabWidget* metaTab;
	virtual void mousePressEvent(QMouseEvent *event);
};

#endif // TAB_H
