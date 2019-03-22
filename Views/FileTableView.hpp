
#ifndef TAB_H
#define TAB_H

//#include <QWidget>
//#include <QListView>
#include <QDir>
#include <QTableView>
#include <QTimer>
#include "Delegates/TableItemDelegate.hpp"
#include "Models/OrderedFileSystemModel.hpp"

// class TableItemDelegate;
class OrderedFileSystemModel;
class QLabel;
class ItemContextMenu;
class FileTabSelector;

class FileTableView : public QTableView {
	Q_OBJECT
protected:
	enum Action { PLUS, MINUS, ASTERISK };

protected:
	FileTableView(QWidget *parent);

public:
	explicit FileTableView(const QDir &directory, QWidget *parent = nullptr);

	//~FileTableView(){delete prevSelection;}

	virtual void init();

	QFileInfoList getSelectedFiles();
	void cdTo(const QString &);
	TableItemDelegate *itemDelegate() const {
		// return qobject_cast<TableItemDelegate *>(QTableView::itemDelegate());
		return qobject_cast<TableItemDelegate *>(QTableView::itemDelegate());
	}
	OrderedFileSystemModel *getModel() const {
		return qobject_cast<OrderedFileSystemModel *>(model);
	}
	QModelIndexList getSelectedIndexes();
	virtual QString getDirectory() const;

	void setTabOrderIndex(int index) { this->index = index; }
	void setLabel(QLabel *infoLabel) { this->infoLabel = infoLabel; }

	void goToFile(const QString &fullFilePath);
	void openEditor(QModelIndex &index);
	void deleteSelectedFiles();
	void showHidden(bool show);

signals:
	// void activated(const QModelIndex &index);
	void dirChanged(const QString dirName, int index);
	void focusEvent(bool);
	void setFileAction(QFileInfoList, QString, Qt::DropAction);
	void contextMenuRequested(QPoint);

public slots:
	virtual void on_doubleClicked(const QModelIndex &index);
	virtual void openContextMenu(QPoint);

	void setCurrentSelection(const QString &);
	virtual void headerClicked(int section);
	void rowsRemoved(const QModelIndex &, int, int);
	void rowsInserted(const QModelIndex &parent, int first, int) override;
	virtual void updateInfo();
	void commitNewName(QWidget *editor);

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void focusInEvent(QFocusEvent *event) override;
	void focusOutEvent(QFocusEvent *event) override;
	virtual bool isCurrent() const;
	virtual void chDir(const QModelIndex &index, bool in_out);

protected:
	int index{};
	int prevRow{-1};
	QLabel *infoLabel{nullptr};
	TableItemDelegate *delegate{nullptr};
	FileTabSelector *parent{nullptr};
	QString directory;
	void queryDialog(QString &filter, Action act);
	QAbstractItemModel *model{};

private:
	ItemContextMenu *menu{nullptr};
	QTimer slowDoubleClickTimer;
	bool editorIsOpen{false};
	bool slowDoubleClick{false};

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void setSelectionAction(Action act);
};

#endif // TAB_H
