#include "ItemContextMenu.hpp"
#include "Views/FileTabView.hpp"

ItemContextMenu::ItemContextMenu(QWidget *parent) : QMenu(parent){

	initFolder();

	initCommon();
	this->parent = (FileTableView*)parent;
	qDebug()<<"parent name: "<<parent->objectName();
	clipboard = QGuiApplication::clipboard();
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
	foreach (auto &format, data->formats()) {
		qDebug()<<format;
		qDebug()<<data->data(format);
	}

	qDebug()<<"****************************";
	if(!clipboard->mimeData()->urls().length())
		pasteAction->setDisabled(true);


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
}
void ItemContextMenu::initFile(){

	//init(QCursor::pos());


	KFileItemActions fileItemActions;
	auto fileInfo = selectedFiles.first();
	qDebug()<<QMimeDatabase().mimeTypeForFile(fileInfo);
	qDebug()<<QMimeDatabase().mimeTypeForFile(fileInfo).name();
	struct stat buf;
	qDebug()<<QMimeDatabase().mimeTypeForFile(fileInfo).name();
	stat(fileInfo.absoluteFilePath().toLocal8Bit().data(), &buf);
	KFileItem kItem(QUrl(fileInfo.absoluteFilePath()),
					QMimeDatabase().mimeTypeForFile(fileInfo).name(), buf.st_mode);
	qDebug()<<kItem;
	QList<KFileItem> kList;
	kList.append(kItem);

	KFileItemListProperties kprops( kList );
	qDebug()<<kprops.items().count();

	fileItemActions.setItemListProperties(kprops);

	QString name("Open with");
	fileItemActions.addOpenWithActionsTo(this,
		QStringLiteral("DesktopEntryName != '%1'").arg(qApp->desktopFileName()));

}
void ItemContextMenu::initFolder(){

}

void ItemContextMenu::cutToClipboard(){
	auto data = parent->getModel()->mimeData(selIndexes);
	data->setData("application/x-kde-cutselection","1");
	qDebug()<<data->formats();
	clipboard->setMimeData(data);
}

void ItemContextMenu::copyToClipboard(){
	auto data = parent->getModel()->mimeData(selIndexes);


	foreach (auto &format, data->formats()) {
		qDebug()<<data->data(format);
	}


	clipboard->setMimeData(data);
	auto data1 = clipboard->mimeData();
	qDebug()<<data1->formats();

	foreach (auto &format, data1->formats()) {
		qDebug()<<data1->data(format);
	}
}

void ItemContextMenu::pasteFromClipboard(){
	auto data = clipboard->mimeData();
	qDebug()<<data->data("application/x-kde-cutselection").length();
	qDebug()<<data->data("application/x-kde-cutselection");
	if( data->data("application/x-kde-cutselection").length() ){
		parent->getModel()->dropMimeData(data, Qt::MoveAction, 1, 0, QModelIndex());
		selIndexes.clear();
	}else
		parent->getModel()->dropMimeData(data, Qt::CopyAction, 1, 0, QModelIndex());
}

void ItemContextMenu::deleteItems(){
	parent->deleteSelectedFiles();
}

void ItemContextMenu::rename(){
	parent->openEditor(selIndexes.first());
}

