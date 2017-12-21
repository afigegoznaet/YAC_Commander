#include "ItemContextMenu.hpp"
#include "Views/FileTabView.hpp"

ItemContextMenu::ItemContextMenu(QWidget *parent) : QMenu(parent){
	initFolder();
	initFile();
	initCommon();
	this->parent = (FileTableView*)parent;
	clipboard = QGuiApplication::clipboard();
}

void ItemContextMenu::init(QPoint &loc){

	sel = parent->getSelectedFiles();

	selIndexes = parent->selectionModel()->selectedRows();

	if(!sel.length()){
		auto index = parent->indexAt(loc);
		auto info = parent->getModel()->fileInfo(index);
		if(!index.isValid()){
			info = parent->getModel()->fileInfo(parent->rootIndex());
			info.setFile(info.absoluteFilePath(), ".");
		}else{
			parent->setCurrentIndex(index);
			sel.append(info);
			selIndexes.append(index);
		}

	}
	if(sel.length() < 2 && !sel.first().fileName().compare(".")){
		cutAction->setDisabled(true);
		copyAction->setDisabled(true);
		renameAction->setDisabled(true);
	}


	if(!clipboard->mimeData()->urls().length())
		pasteAction->setDisabled(true);

	addSeparator();
}

void ItemContextMenu::initCommon(){

	cutAction = addAction("Cut", this, &ItemContextMenu::cutToClipboard,
								QKeySequence(tr("Ctrl+X")));
	copyAction = addAction("Copy", this, &ItemContextMenu::copyToClipboard,
								QKeySequence(tr("Ctrl+C")));
	pasteAction = addAction("Paste", this, &ItemContextMenu::pasteFromClipboard,
								QKeySequence(tr("Ctrl+V")));

	deleteAction = addAction("Delete selected", this, &ItemContextMenu::deleteItems,
								QKeySequence(tr("Del")));
	renameAction = addAction("Rename", this, &ItemContextMenu::rename,
								QKeySequence(tr("Ins")));
}
void ItemContextMenu::initFile(){

}
void ItemContextMenu::initFolder(){

}

void ItemContextMenu::cutToClipboard(){
	auto data = parent->getModel()->mimeData(selIndexes);
	clipboard->setMimeData(data);
}

void ItemContextMenu::copyToClipboard(){
	auto data = parent->getModel()->mimeData(selIndexes);
	clipboard->setMimeData(data);
}

void ItemContextMenu::pasteFromClipboard(){
	parent->getModel()->dropMimeData(clipboard->mimeData(), Qt::CopyAction, 1, 0, QModelIndex());
}

void ItemContextMenu::deleteItems(){}

void ItemContextMenu::rename(){}

