#ifndef ITEMCONTEXTMENU_HPP
#define ITEMCONTEXTMENU_HPP

#include <QMenu>
#include <QFileInfo>
#include <QDebug>
#include <QClipboard>
#include <QModelIndexList>

class FileTableView;
class ItemContextMenu : public QMenu
{
	Q_OBJECT
public:
	explicit ItemContextMenu(QWidget *parent = nullptr);
	void init(QPoint& loc);

signals:

public slots:

private:
	QList<QAction*> commonActions;
	QList<QAction*> folderActions;
	QList<QAction*> fileActions;
	QAction* pasteAction;
	QAction* copyAction;
	QAction* cutAction;
	QAction* deleteAction;
	QAction* renameAction;
	FileTableView* parent;
	QClipboard* clipboard;
	QFileInfoList sel;
	QModelIndexList selIndexes;

	void initCommon();
	void initFile();
	void initFolder();
	void cutToClipboard();
	void copyToClipboard();
	void pasteFromClipboard();
	void deleteItems();
	void rename();
};

#endif // ITEMCONTEXTMENU_HPP
