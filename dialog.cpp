#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	progress(new Ui::Dialog){
	progress->setupUi(this);
	progress->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	progress->tableWidget->setColumnCount(3);
	progress->tableWidget->verticalHeader()->setDefaultSectionSize(this->fontMetrics().height()+6);

	progress->tableWidget->horizontalHeader()->setStretchLastSection(true);
	progress->tableWidget->horizontalHeader()->setSectionsMovable(true);
	progress->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

	QStringList m_TableHeader;
	m_TableHeader<<"#"<<"Name"<<"Text";
	progress->tableWidget->setHorizontalHeaderLabels(m_TableHeader);
}

Dialog::~Dialog(){
	QSettings settings;
	settings.setValue("ProgressColumns", progress->tableWidget->horizontalHeader()->saveState());
	delete progress;
}

void Dialog::setFileAction(QFileInfoList fileList, QString destination, ACTION action){
	int newRow = progress->tableWidget->rowCount();

	switch(action){
	case MOVE:
		foreach (auto fileInfo, fileList) {
			QString item = "Move "+ fileInfo.fileName() + " to "+destination;
			QString newName = destination+"/"+fileInfo.fileName();
			//QListWidgetItem item;
			//item.setText(fileInfo.fileName());


			progress->tableWidget->insertRow( newRow );
			progress->tableWidget->setItem(newRow,0,new QTableWidgetItem("Move"));
			progress->tableWidget->setItem(newRow,1,new QTableWidgetItem(fileInfo.baseName()));
			progress->tableWidget->setItem(newRow,2,new QTableWidgetItem(destination));

			progressList.append(fileInfo);
			//newDialog.setFileAction(fileList, destination);
			//QFile::rename(fileInfo.absoluteFilePath(), newName);
		}
		break;
	case COPY:
		foreach (auto fileInfo, fileList) {
			QString newName = destination+"/"+fileInfo.fileName();
			//progress->tableWidget->addItem(fileInfo.fileName());
			progress->tableWidget->insertRow( newRow );
			progress->tableWidget->setItem(newRow,0,new QTableWidgetItem("Copy"));
			progress->tableWidget->setItem(newRow,1,new QTableWidgetItem(fileInfo.baseName()));
			progress->tableWidget->setItem(newRow,2,new QTableWidgetItem(destination));
			progressList.append(fileInfo);
		}
	}

}

void Dialog::onWrite( qint64 bytesWritten){
	writtenKb+=bytesWritten;
	progress->progressBar->setValue(writtenKb);
}

void Dialog::DoSomething(){
	while (progress->tableWidget->rowCount()) {
		QFile from(progressList.front().absoluteFilePath());
		QString destination( progress->tableWidget->item(0,2)->text() );
		destination.append(progress->tableWidget->item(0,1)->text());
		QFile to(destination);
		progressList.pop_front();
		progress->tableWidget->removeRow(0);
		writtenKb = 0;
		progress->progressBar->setMaximum( progressList.front().size() / 1024 );
	}
}
