#include "dropdown.h"

DropDown::DropDown(QWidget *parent) : QComboBox(parent) {
	qDebug()<<"helo";
}


void DropDown::keyPressEvent(QKeyEvent *event){
	auto key = event->key();
	switch (key) {
		default:
			QComboBox::keyPressEvent(event);
			break;
		case Qt::Key_Return:
		case Qt::Key_Enter:
			qDebug()<<lineEdit()->text();
			break;
	}
}
