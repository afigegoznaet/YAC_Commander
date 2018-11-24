
#ifndef TAB_H
#define TAB_H

//#include <QWidget>
//#include <QListView>
#include <QTableView>
#include <QDir>
#include <QTimer>

class TableItemDelegate;
class OrderedFileSystemModel;
class QLabel;
class ItemContextMenu;

class FileTableView : public QTableView {
	Q_OBJECT

	enum Action { PLUS, MINUS, ASTERISK };

public:
	explicit FileTableView(const QDir &directory, QWidget *parent = 0);
	FileTableView(QWidget *parent) : FileTableView(QDir::homePath(), parent) {}
	//~FileTableView(){delete prevSelection;}

	void init();

	QFileInfoList getSelectedFiles();
	void cdTo(const QString &);
	TableItemDelegate *itemDelegate() const {
		// return qobject_cast<TableItemDelegate *>(QTableView::itemDelegate());
		return (TableItemDelegate *)QTableView::itemDelegate();
	}
	OrderedFileSystemModel *getModel() { return model; }
	QModelIndexList getSelectedIndexes();
	QString getDirectory();
	QLabel *getLabel() { return infoLabel; }
	void setTabOrderIndex(int index) { this->index = index; }
	void setLabel(QLabel *infoLabel) { this->infoLabel = infoLabel; }


	void goToFile(QString &fullFilePath);
	void openEditor(QModelIndex &index);
	void deleteSelectedFiles();
	void showHidden(bool show);
	OrderedFileSystemModel *getModel() const { return model; }

signals:
	// void activated(const QModelIndex &index);
	void dirChanged(const QString dirName, int index);
	void focusEvent(bool);
	void setInfo();
	void setFileAction(QFileInfoList, QString, Qt::DropAction);
	void contextMenuRequested(QPoint);


public slots:
	void on_doubleClicked(const QModelIndex &index);
	void setCurrentSelection(const QString &);
	void headerClicked(int section);
	void rowsRemoved(const QModelIndex &, int, int);
	void rowsInserted(const QModelIndex &parent, int first, int) override;
	void updateInfo();
	void openContextMenu(QPoint);
	void commitNewName(QWidget *editor);

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void chDir(const QModelIndex &index, bool in_out);
	void focusInEvent(QFocusEvent *event) override;
	void focusOutEvent(QFocusEvent *event) override;

private:
	int index{};
	int prevRow = -1;
	bool editorIsOpen = false;
	bool slowDoubleClick = false;
	QLabel *infoLabel = nullptr;
	QTabWidget *metaTab = nullptr;
	TableItemDelegate *delegate = nullptr;
	ItemContextMenu *menu;
	QString directory;
	QTimer slowDoubleClickTimer;
	OrderedFileSystemModel *model{};

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void queryDialog(QString &filter, Action act);
	void setSelectionAction(Action act);
};

#endif // TAB_H
