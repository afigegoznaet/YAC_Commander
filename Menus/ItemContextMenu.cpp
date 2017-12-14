#include "ItemContextMenu.hpp"

ItemContextMenu::ItemContextMenu(QWidget *parent) : QMenu(parent){
	addAction("Add active filter (N/A yet)");
	addAction("Add digital filter (N/A yet)");
	addAction("Add buffer/amplifier (N/A yet)");
	addSeparator();
	addAction("Place text (N/A yet)");
	addSeparator();
}
