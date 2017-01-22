#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	progress(new Ui::Dialog)
{
	progress->setupUi(this);
}

Dialog::~Dialog()
{
	delete progress;
}

void Dialog::setFileAction(QFileInfoList fileList, QString destination, ACTION action){
	switch(action){
	case MOVE:
		foreach (auto fileInfo, fileList) {
			QString item = "Move "+ fileInfo.fileName() + " to "+destination;
			QString newName = destination+"/"+fileInfo.fileName();
			//QListWidgetItem item;
			//item.setText(fileInfo.fileName());

			progress->listWidget->addItem(item);
			//newDialog.setFileAction(fileList, destination);
			//QFile::rename(fileInfo.absoluteFilePath(), newName);
		}
		break;
	case COPY:
		foreach (auto fileInfo, fileList) {
			QString newName = destination+"/"+fileInfo.fileName();
			progress->listWidget->addItem(fileInfo.fileName());
			//newDialog.setFileAction(fileList, destination);
			//QFile::copy(fileInfo.absoluteFilePath(), newName);
		}
	}

}
