#include "dropdown.h"
#include "mainwindow.h"

DropDown::DropDown(QWidget *parent) : QComboBox(parent) {
	qDebug()<<"helo";

}

void DropDown::setMain(MainWindow *mainWindow){
	this->mainWindow=mainWindow;
	//readSettings();
	QTimer::singleShot(200, this, &DropDown::clearEditText);	//An ugly hack - never use it
}

void DropDown::processCommand(){
	QString cmd(lineEdit()->text());
	emit focusPreviouslyFocused();
	if(findText(cmd)<0)
		insertItem(0,cmd);
	if(cmd.startsWith("cd")){
		cmd.remove(0,3);
		emit cdTo(cmd.trimmed());
	}else{
		QProcess proc;
		QString dir(mainWindow->getDirInFocus());
		//QDir::setCurrent(dir);
		QStringList args;
#ifdef _WIN32
		args = cmd.split(" ");
		QString program = args.first();
		args.removeFirst();
#else
		args << "-exec";
		args.append(cmd);
		QString program = "sh";
		//args.removeFirst();
#endif
		qDebug()<<proc.startDetached(program, args, dir);
		//qDebug()<<proc.execute(program, args);
	}

	clearEditText();
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


DropDown::~DropDown(){

	QSettings settings;
	int count=this->count();
	settings.beginWriteArray("Commands",count);
	for(int i=0;i<count && i<50;i++){
		settings.setArrayIndex(i);
		settings.setValue("command", itemText(i));
	}
	settings.endArray();
}

void DropDown::readSettings(){
	//return;
	QSettings settings;
	int count = settings.beginReadArray("Commands");
	int i=0;
	while (count-- > 0) {
		settings.setArrayIndex(i);
		insertItem(i++,settings.value("command").toString());
	};
	clearEditText();
	//insertItem(0,"");
}
