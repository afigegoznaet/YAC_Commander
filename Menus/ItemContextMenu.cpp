#include "ItemContextMenu.hpp"
#include "Views/FileTabView.hpp"


ItemContextMenu::ItemContextMenu(QWidget *parent) : QMenu(parent){

	initCommon();
	this->parent = (FileTableView*)parent;
	qDebug()<<"parent name: "<<parent->objectName();
	clipboard = QGuiApplication::clipboard();
	connect(this, &QMenu::aboutToHide, [&](){
#ifdef __linux__
		qDebug()<<fileItemActions->children();
		fileItemActions->deleteLater();
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

	selIndexes = parent->selectionModel()->selectedRows();

	if(!selIndexes.length()){
		auto index = parent->indexAt(loc);
		auto info = parent->getModel()->fileInfo(index);
		if(!index.isValid()){
			info = parent->getModel()->fileInfo(parent->rootIndex());
			info.setFile(info.absoluteFilePath(), ".");
		}else{
			parent->setCurrentIndex(index);
			selectedFiles.append(info);
			selIndexes.append(index);
		}

	}
	if(selectedFiles.length() < 2 && !selectedFiles.first().fileName().compare("..")){
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


	fileItemActions = new KFileItemActions(this);

	fileItemActions->setParentWidget(this);

	QList<KFileItem> kList;
	for(const auto fileInfo : selectedFiles){
		struct stat buf;
		stat(fileInfo.absoluteFilePath().toLocal8Bit().data(), &buf);
		KFileItem kItem(QUrl(fileInfo.absoluteFilePath()),
						QMimeDatabase().mimeTypeForFile(fileInfo).name(), buf.st_mode);
		//qDebug()<<kItem;
		kList.append(kItem);
	}


	KFileItemListProperties kprops( kList );
	//qDebug()<<kprops.items().count();

	fileItemActions->setItemListProperties(kprops);

	//qDebug()<<KAuthorized::authorize("Compress7z");
	//qDebug()<<KAuthorized::authorize("CompressDialog");
	qDebug()<<QStringLiteral("DesktopEntryName != '%1'").arg(qApp->desktopFileName());
	qDebug()<<qApp->desktopFileName();
	fileItemActions->addServiceActionsTo(this);
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
	qDebug()<<data->formats();
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
		qDebug()<<url;
		auto text = data->data(url);
		qDebug()<<text;
		qDebug()<<"*********************************************************";
	}

	auto status = data->data("application/x-qt-windows-mime;value=\"Preferred DropEffect\"");
	qDebug()<<status;

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

