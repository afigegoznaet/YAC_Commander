#include "progressDialog.h"
#include "ui_progressDialog.h"
#include <QDebug>
#include <windows.h> // for Sleep
#include "filemover.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
	QDialog(parent),
	progress(new Ui::ProgressDialog){
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

ProgressDialog::~ProgressDialog(){
	QSettings settings;
	settings.setValue("ProgressColumns", progress->tableWidget->horizontalHeader()->saveState());
	delete progress;
}

void ProgressDialog::setFileAction(QFileInfoList fileList, QString destination, ACTION action){
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

	DoSomething();
}

void ProgressDialog::onWrite( qint64 bytesWritten){
	writtenKb+=bytesWritten;
	progress->progressBar->setValue(writtenKb);
}

void ProgressDialog::DoSomething(){
	if (progress->tableWidget->rowCount()) {
		Sleep(1000);
		QFile from(progressList.front().absoluteFilePath());
		QString destination( progress->tableWidget->item(0,2)->text() );
		destination.append("/");
		destination.append(progress->tableWidget->item(0,1)->text());
		qDebug()<<destination;
		QString action =  progress->tableWidget->item(0,0)->text();
		writtenKb = 0;
		progress->progressBar->setMaximum( progressList.front().size() / 1024 );
		new FileMover(from, destination, action, this);
		//from.copy(destination);
		//progressList.pop_front();
		//progress->tableWidget->removeRow(0);
	}else{
		this->hide();
	}
}
