#ifndef ITEMCONTEXTMENU_HPP
#define ITEMCONTEXTMENU_HPP

#include <QMenu>
#include <QFileInfo>
#include <QDebug>
class ItemContextMenu : public QMenu
{
	Q_OBJECT
public:
	explicit ItemContextMenu(QWidget *parent = nullptr);
	void init(QFileInfo& info);

signals:

public slots:

private:
	QList<QAction*> commonActions;
	QList<QAction*> folderActions;
	QList<QAction*> fileActions;
	QFileInfo info;
	QAction* pasteAction;
	QAction* copyAction;
	QAction* cutAction;
	QAction* renameAction;

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
