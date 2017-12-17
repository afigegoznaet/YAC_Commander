#include "ItemContextMenu.hpp"

ItemContextMenu::ItemContextMenu(QWidget *parent) : QMenu(parent){
	initFolder();
	initFile();
	initCommon();
}

void ItemContextMenu::init(QFileInfo& info){
	this->info = std::move(info);

	if(!this->info.fileName().compare(".")){
		cutAction->setDisabled(true);
		copyAction->setDisabled(true);
		renameAction->setDisabled(true);
	}
	qDebug()<<info.fileName();
	addSeparator();
}

void ItemContextMenu::initCommon(){

	cutAction = addAction("Cut", this, &ItemContextMenu::cutToClipboard,
								QKeySequence(tr("Ctrl+X")));
	copyAction = addAction("Copy", this, &ItemContextMenu::copyToClipboard,
								QKeySequence(tr("Ctrl+C")));
	pasteAction = addAction("Paste", this, &ItemContextMenu::pasteFromClipboard,
								QKeySequence(tr("Ctrl+V")));

	fileActions << addAction("Delete selected", this, &ItemContextMenu::deleteItems,
								QKeySequence(tr("Del")));
	renameAction = addAction("Rename", this, &ItemContextMenu::rename,
								QKeySequence(tr("Ins")));
}
void ItemContextMenu::initFile(){

}
void ItemContextMenu::initFolder(){

}

void ItemContextMenu::cutToClipboard(){}

void ItemContextMenu::copyToClipboard(){}

void ItemContextMenu::pasteFromClipboard(){}

void ItemContextMenu::deleteItems(){}

void ItemContextMenu::rename(){}
