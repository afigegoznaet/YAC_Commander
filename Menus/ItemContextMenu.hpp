#ifndef ITEMCONTEXTMENU_HPP
#define ITEMCONTEXTMENU_HPP

#include <QMenu>
#include <QFileInfo>
#include <QModelIndexList>
#ifdef __linux__
#include <KFileItem>
#endif

class KXmlGuiWindow;
class FileTableView;
class ItemContextMenu : public QMenu {
	Q_OBJECT
public:
	explicit ItemContextMenu(QWidget *parent = nullptr);
	void init();

private:
	QByteArray cutActionIndicator;
	QByteArray cutActionPadding;

signals:

public slots:


private:
	QList<QAction *> commonActions;
	QList<QAction *> folderActions;
	QList<QAction *> fileActions;
	QAction *		 pasteAction{};
	QAction *		 copyAction{};
	QAction *		 cutAction{};
	QAction *		 deleteAction{};
	QAction *		 renameAction{};
	FileTableView *  parent;
	// QClipboard* clipboard;
	QFileInfoList   selectedFiles;
	QModelIndexList selIndexes;

#ifdef __linux__
	KXmlGuiWindow *gui;
	// KFileItemActions* fileItemActions;
#endif

	void initCommon();
	void initFile();
	void initFolder();

	void cutToClipboard();
	void copyToClipboard();
	void pasteFromClipboard();
	void deleteItems();
	void rename();

#ifdef __linux__
	void initProperties(KFileItemList &&);
#endif
};

#endif // ITEMCONTEXTMENU_HPP
