#include "EditableDropdown.hpp"
#include "Dialogs/FileSearchDlg.hpp"
#include <QKeyEvent>

EditableDropDown::EditableDropDown(QWidget *parent) : QComboBox(parent) {}

void EditableDropDown::processCommand() {
	auto *parent = dynamic_cast<SearchDialog *>(parentDlg);
	if (parent)
		parent->on_searchButton_clicked();
}

void EditableDropDown::keyPressEvent(QKeyEvent *event) {
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
