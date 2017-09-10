#include "customdropdown.h"
#include "searchdialog.h"

CustomDropDown::CustomDropDown(QWidget *parent) : QComboBox(parent) {

}


void CustomDropDown::processCommand(){
	SearchDialog* parent = dynamic_cast<SearchDialog*>(parentDlg);
	if(parent)
		parent->startSearch();
}

void CustomDropDown::keyPressEvent(QKeyEvent *event){
	auto key = event->key();
	switch (key) {
		default:
			QComboBox::keyPressEvent(event);
			break;
		case Qt::Key_Return:
		case Qt::Key_Enter:
			processCommand();
			break;
	}
}
