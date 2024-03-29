
#ifndef TAB_H
#define TAB_H

// #include <QWidget>
// #include <QListView>
#include <QDir>
#include <QTableView>
#include <QTimer>
#include <QFileSystemWatcher>

class TableItemDelegate;
class OrderedFileSystemModel;
class QLabel;
class ItemContextMenu;
class FileTabSelector;

class FileTableView : public QTableView {
	Q_OBJECT

	enum Action { PLUS,
				  MINUS,
				  ASTERISK };

public:
	explicit FileTableView(QDir &&directory, QWidget *parent = nullptr);
	FileTableView(QWidget *parent) : FileTableView(QDir(QDir::homePath()), parent) {}
	~FileTableView() override;

	void init();

	QFileInfoList					 getSelectedFiles();
	void							 cdTo(const QString &);
	[[nodiscard]] TableItemDelegate *itemDelegate() const;
	OrderedFileSystemModel			*getModel() { return model; }
	QModelIndexList					 getSelectedIndexes();
	QString							 getDirectory();
	// QLabel *getLabel() { return infoLabel; }
	void setTabOrderIndex(int index) { this->index = index; }
	void setLabel(QLabel *infoLabel) { this->infoLabel = infoLabel; }

	void								  goToFile(const QString &fullFilePath);
	void								  openEditor(QModelIndex &index);
	void								  deleteSelectedFiles();
	void								  showHidden(bool show);
	[[nodiscard]] OrderedFileSystemModel *getModel() const { return model; }

signals:
	// void activated(const QModelIndex &index);
	void dirChanged(QString dirName, int index);
	void focusEvent(bool);
	void setFileAction(QFileInfoList, QString, Qt::DropAction);
	void contextMenuRequested(QPoint);

public slots:
	void on_doubleClicked(const QModelIndex &index);
	void setCurrentSelection(const QString &);
	void headerClicked(int section);
	void rowsRemoved(const QModelIndex &, int first, int last);
	void rowsInserted(const QModelIndex &parent, int first, int) override;
	void updateInfo();
	void openContextMenu(QPoint);
	void commitNewName(QWidget *editor);
	void fileWatcher(const QString &);

protected:
	void			   keyPressEvent(QKeyEvent *event) override;
	void			   chDir(const QModelIndex &index, bool in_out);
	void			   focusInEvent(QFocusEvent *event) override;
	void			   focusOutEvent(QFocusEvent *event) override;
	[[nodiscard]] bool isCurrent() const;

private:
	int						index{};
	int						prevRow = -1;
	bool					editorIsOpen = false;
	bool					slowDoubleClick = false;
	QLabel				   *infoLabel = nullptr;
	TableItemDelegate	   *delegate = nullptr;
	ItemContextMenu		   *menu;
	QString					directory;
	QTimer					slowDoubleClickTimer;
	OrderedFileSystemModel *model{};
	FileTabSelector		   *parent = nullptr;

	QFileSystemWatcher dirWatch;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void queryDialog(QString &filter, Action act);
	void setSelectionAction(Action act);
};

#endif // TAB_H
