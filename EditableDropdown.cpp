#include "EditableDropdown.h"
#include "searchdialog.h"

EditableDropDown::EditableDropDown(QWidget *parent) : QComboBox(parent) {

}


void EditableDropDown::processCommand(){
	SearchDialog* parent = dynamic_cast<SearchDialog*>(parentDlg);
	if(parent)
		parent->on_searchButton_clicked();
}

void EditableDropDown::keyPressEvent(QKeyEvent *event){
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
