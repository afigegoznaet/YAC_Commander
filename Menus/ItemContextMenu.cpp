#include "ItemContextMenu.hpp"
#include "Views/FileTabView.hpp"
#include <KConfigGroup>
#include <KSharedConfig>

ItemContextMenu::ItemContextMenu(QWidget *parent) : QMenu(parent){

	initCommon();
	this->parent = (FileTableView*)parent;
	//qDebug()<<"parent name: "<<parent->objectName();
	clipboard = QGuiApplication::clipboard();
	connect(this, &QMenu::aboutToHide, [&](){
#ifdef __linux__
		//qDebug()<<fileItemActions->children();
		//fileItemActions->deleteLater();
#endif
			for(auto action: this->actions())
				if(commonActions.contains(action))
					continue;
				else
					removeAction(action);

		;});

	cutActionIndicator.reserve(4);
	cutActionIndicator[0] = 2;
	cutActionIndicator[1] = '\0';
	cutActionIndicator[2] = '\0';
	cutActionIndicator[3] = '\0';
	cutActionPadding.reserve(1044);
	cutActionPadding[0] = 255;
	cutActionPadding[1] = 255;
	cutActionPadding[2] = 255;
	cutActionPadding[3] = 255;
}

void ItemContextMenu::init(QPoint loc){


	selectedFiles = parent->getSelectedFiles();

	if(selectedFiles.count()<1)
		selectedFiles.append(parent->getModel()->fileInfo(parent->indexAt(loc)));


	if(!selIndexes.length()){
		auto index = parent->indexAt(loc);
		//auto info = parent->getModel()->fileInfo(index);
		if(index.isValid())
			selIndexes.append(index);
	}

	if(selectedFiles.length() == 1 &&
			 (!selectedFiles.first().fileName().compare("..") ||
				selectedFiles.first().fileName().isEmpty())){
		cutAction->setDisabled(true);
		copyAction->setDisabled(true);
		renameAction->setDisabled(true);
		deleteAction->setDisabled(true);
	}

	auto data = clipboard->mimeData();
	foreach (auto &url, data->formats()) {
		qDebug()<<url;
		auto text = data->data(url);
		qDebug()<<text;
		qDebug()<<"*********************************************************";
	}

	bool hasUrls = data->hasUrls();
	if(!hasUrls)
		pasteAction->setDisabled(true);
	else
		pasteAction->setEnabled(true);

	initFolder();
	initFile();

}

void ItemContextMenu::initCommon(){

	cutAction = addAction("Cut", this, &ItemContextMenu::cutToClipboard,
								QKeySequence(tr("Ctrl+X")));
	copyAction = addAction("Copy", this, &ItemContextMenu::copyToClipboard,
								QKeySequence(tr("Ctrl+C")));
	pasteAction = addAction("Paste", this, &ItemContextMenu::pasteFromClipboard,
								QKeySequence(tr("Ctrl+V")));

	addSeparator();
	deleteAction = addAction("Delete selected", this, &ItemContextMenu::deleteItems,
								QKeySequence(tr("Del")));
	renameAction = addAction("Rename", this, &ItemContextMenu::rename,
								QKeySequence(tr("Ins")));
	addSeparator();
	commonActions << pasteAction << copyAction << cutAction << deleteAction << renameAction;
}
void ItemContextMenu::initFile(){

	//init(QCursor::pos());
#ifdef __linux__

	KServiceAction action;


	KFileItemActions* fileItemActions = new KFileItemActions(this);

	fileItemActions->setParentWidget(this);

	//qDebug()<<"Adding files to list";
	KFileItemList kList;
	for(const auto fileInfo : selectedFiles){
		struct stat buf;
		stat(fileInfo.absoluteFilePath().toLocal8Bit().data(), &buf);
		KFileItem kItem(QUrl::fromLocalFile(fileInfo.absoluteFilePath()),
						QMimeDatabase().mimeTypeForFile(fileInfo).name(),
						buf.st_mode);
		//qDebug()<<"KFileItem path: "<<kItem.localPath();
		//qDebug()<<fileInfo.fileName();
		//qDebug()<<QMimeDatabase().mimeTypeForFile(fileInfo).name();
		//qDebug()<<kItem.currentMimeType();
		//qDebug()<<kItem.determineMimeType();
		//qDebug()<<kItem.isReadable();
		//qDebug()<<kItem.isWritable();
		kList.append(kItem);
	}
/*
	for(const auto& item : kList){
		qDebug()<<item.isReadable();
		qDebug()<<item.isRegularFile();
		qDebug()<<item.isWritable();
		qDebug()<<item.localPath();
		qDebug()<<item.mode();
		qDebug()<<item.permissionsString();
		qDebug()<<item.size();
	}

	qDebug()<<kList.count();

	KFileItemList testList(kList);
	for(const auto& item : testList){
		qDebug()<<item.name();
		qDebug()<<item.isReadable();
		qDebug()<<item.isWritable();
	}
*/
	KFileItemListProperties kprops( kList );

	//qDebug()<<kprops.mimeGroup();
	//qDebug()<<kprops.mimeType();
	//qDebug()<<kprops.supportsDeleting();
	//qDebug()<<kprops.supportsMoving();
	//qDebug()<<kprops.supportsReading();
	//qDebug()<<kprops.supportsWriting();
	//qDebug()<<kprops.urlList();

	//qDebug()<<kprops.items().count();

	fileItemActions->setItemListProperties(kprops);

	//KConfigGroup cg(KSharedConfig::openConfig(), "KDE Action Restrictions");

	//qDebug()<<cg.keyList();

	//qDebug()<<KAuthorized::authorize(QStringLiteral("shell_access"));
	//qDebug()<<KAuthorized::authorize(QStringLiteral("Compress"));
	//qDebug()<<KAuthorized::authorize(QStringLiteral("Extract"));
	//qDebug()<<KAuthorized::authorize(QStringLiteral("Write"));
	//qDebug()<<KAuthorized::authorizeAction("Extract");
	//qDebug()<<KAuthorized::authorizeControlModule(QStringLiteral("org.kde.ark.desktop"));
	//qDebug()<<KAuthorized::authorizeControlModule(QStringLiteral("kcmtrash.desktop"));
	//qDebug()<<KAuthorized::authorize("CompressDialog");
	//qDebug()<<QStringLiteral("DesktopEntryName != '%1'").arg(qApp->desktopFileName());
	//qDebug()<<qApp->desktopFileName();

	//QString name("Open with");
	fileItemActions->addOpenWithActionsTo(this,
		QStringLiteral("DesktopEntryName != '%1'").arg(qApp->desktopFileName()));
	fileItemActions->addServiceActionsTo(this);
	fileItemActions->addPluginActionsTo(this);

#endif
}
void ItemContextMenu::initFolder(){

}

void ItemContextMenu::cutToClipboard(){
	auto data = parent->getModel()->mimeData(selIndexes);
#ifdef WIN32
	data->setData("application/x-qt-windows-mime;value=\"Preferred DropEffect\"",cutActionIndicator);
	data->setData("application/x-qt-windows-mime;value=\"DropDescription\"",cutActionPadding);
#else
	data->setData("application/x-kde-cutselection","1");
#endif
	//qDebug()<<data->formats();
	clipboard->setMimeData(data);
}

void ItemContextMenu::copyToClipboard(){
	auto data = parent->getModel()->mimeData(selIndexes);

	clipboard->setMimeData(data);

	bool hasUrls = clipboard->mimeData()->hasUrls();

	if( !hasUrls )
		pasteAction->setDisabled(true);
	else
		pasteAction->setEnabled(true);

}

void ItemContextMenu::pasteFromClipboard(){
	auto data = clipboard->mimeData();

	foreach (auto &url, data->urls()) {
		qDebug()<<url;
	}

	foreach (auto &url, data->formats()) {
		//qDebug()<<url;
		auto text = data->data(url);
		//qDebug()<<text;
		//qDebug()<<"*********************************************************";
	}

	auto status = data->data("application/x-qt-windows-mime;value=\"Preferred DropEffect\"");
	//qDebug()<<status;

	bool move = false;
#ifdef WIN32
	move = (data->data("application/x-qt-windows-mime;value=\"Preferred DropEffect\"").at(0) == 2);
#else
	move = data->data("application/x-kde-cutselection").length() >0;
#endif
	if( move ){
		parent->getModel()->dropMimeData(data, Qt::MoveAction, 1, 0, QModelIndex());
		selIndexes.clear();
		pasteAction->setDisabled(true);
	}else
		parent->getModel()->dropMimeData(data, Qt::CopyAction, 1, 0, QModelIndex());

}

void ItemContextMenu::deleteItems(){
	parent->deleteSelectedFiles();
}

void ItemContextMenu::rename(){
	parent->openEditor(selIndexes.first());
}

