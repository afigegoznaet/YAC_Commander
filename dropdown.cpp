#include "dropdown.h"
#include "mainwindow.h"

DropDown::DropDown(QWidget *parent) : QComboBox(parent) {
	qDebug()<<"helo";
}

void DropDown::setMain(MainWindow *mainWindow){
	this->mainWindow=mainWindow;
}

void DropDown::processCommand(){
	QString cmd(lineEdit()->text());
	focusPreviousChild();
	if(findText(cmd)<0)
		insertItem(0,cmd);
	if(cmd.startsWith("cd")){
		cmd.remove(0,3);
		emit cdTo(cmd.trimmed());
	}else{
		QProcess proc;
		QString dir(mainWindow->getDirInFocus());
		proc.setWorkingDirectory(dir);

		qDebug()<<cmd;
		QStringList args = cmd.split(" ");
		args.removeFirst();
		qDebug()<<args;
		qDebug()<<"Object status: "<<proc.execute(cmd,args);

	}

	lineEdit()->setText("");
}

void DropDown::keyPressEvent(QKeyEvent *event){
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
