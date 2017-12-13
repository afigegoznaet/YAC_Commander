#include "CommandDropdown.hpp"
#include "mainwindow.hpp"

CommandDropDown::CommandDropDown(QWidget *parent) : QComboBox(parent) {
	qDebug()<<"helo";

}

void CommandDropDown::setMain(MainWindow *mainWindow){
	this->mainWindow=mainWindow;
	//readSettings();
    QTimer::singleShot(200, this, &CommandDropDown::clearEditText);	//An ugly hack - never use it
}

void CommandDropDown::processCommand(){
	QString cmd(lineEdit()->text());

    QStandardItemModel* stModel = (QStandardItemModel*)model();
    auto index = findText(cmd);
    if(index<0)
		insertItem(0,cmd);
    else{
        stModel->insertRow(0, stModel->takeRow(index));
    }
	if(cmd.startsWith("cd")){
		cmd.remove(0,3);
		emit cdTo(cmd.trimmed());
	}else{
		QProcess proc;
		QString dir(mainWindow->getDirInFocus());
		//QDir::setCurrent(dir);
		QString program;
		QStringList args;
#ifdef _WIN32

		if(cmd.startsWith("\"")){
			args = cmd.split("\" ");
			program = args.first()+"\"";
		}else{
			args = cmd.split(" ");
			program = args.first();
		}
		args.removeFirst();
#else
		args << "-exec";
		args.append(cmd);
		program = "sh";
		//args.removeFirst();
#endif
		qDebug()<<proc.startDetached(program, args, dir);
		//qDebug()<<proc.execute(program, args);
	}

	clearEditText();
	emit focusPreviouslyFocused();
}

void CommandDropDown::keyPressEvent(QKeyEvent *event){
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


CommandDropDown::~CommandDropDown(){

	QSettings settings;
	int count=this->count();
	settings.beginWriteArray("Commands",count);
	for(int i=0;i<count && i<50;i++){
		settings.setArrayIndex(i);
		settings.setValue("command", itemText(i));
	}
	settings.endArray();
}

void CommandDropDown::readSettings(){
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
