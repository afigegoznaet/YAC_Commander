
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
#include <QLabel>
#include <QTimer>
#include <QStorageInfo>
#include <QGuiApplication>
#include "Models/OrderedFileSystemModel.hpp"
#include "Delegates/TableItemDelegate.hpp"
#include "Menus/ItemContextMenu.hpp"

class FileTableView : public QTableView
{
	Q_OBJECT

	enum Action{PLUS, MINUS, ASTERISK};
public:
	explicit FileTableView(QDir directory, QWidget *parent = 0);
	FileTableView(QWidget *parent) : FileTableView(QDir::homePath(),parent){}
	~FileTableView(){delete prevSelection;}
	QString GetDirectory(){
		return model->rootPath();
	}
	void init();
	void setTabOrderIndex(int index){
		this->index=index;
	}
	QFileInfoList getSelectedFiles();
	void cdTo(const QString&);
	TableItemDelegate *itemDelegate() const{
		return (TableItemDelegate*)QTableView::itemDelegate();
	}
	void setLabel(QLabel* infoLabel){this->infoLabel = infoLabel;}
	QLabel* getLabel(){return infoLabel;}
	void goToFile(QString& fullFilePath);
    OrderedFileSystemModel*  getModel(){
        return model;
    }

signals:
	//void activated(const QModelIndex &index);
	void dirChanged(const QString dirName, int index);
	void focusEvent(bool);
	void setInfo();
	void setFileAction(QFileInfoList, QString, ACTION);
	void contextMenuRequested(QPoint);


public slots:
	void on_doubleClicked(const QModelIndex &index);
	void setCurrentSelection(QString);
	void headerClicked(int section);
	void rowsRemoved(const QModelIndex &, int first, int);
	void rowsInserted(const QModelIndex &parent, int first, int);
	void updateInfo();
	void openContextMenu(QPoint loc);
	void commitingData(QWidget* editor);

protected:
	virtual void keyPressEvent(QKeyEvent * event);
	void chDir(const QModelIndex &index, bool in_out);
	virtual void focusInEvent(QFocusEvent* event);
	virtual void focusOutEvent(QFocusEvent* event);


private:
	QString directory;
	OrderedFileSystemModel* model;
	int index;
	QLabel* infoLabel = nullptr;
	QTabWidget* metaTab = nullptr;
	QModelIndex* prevSelection = nullptr;
	TableItemDelegate* delegate = nullptr;
	bool editorIsOpen = false;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void queryDialog(QString& filter, Action act);
	void setSelection(Action act);
};

#endif // TAB_H

