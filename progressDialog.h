#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileInfoList>
#include <QSettings>
#include "ui_progressDialog.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include "filemover.h"

enum ACTION {COPY, MOVE, LN, LN_S, ENUM_TERMINATOR} ;

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
	Q_OBJECT

	friend class MainWindow;
public:
	explicit ProgressDialog(QWidget *parent = 0);
	~ProgressDialog();
	void setFileAction(QFileInfoList fileList, QString destination, ACTION action);

private:
	Ui::ProgressDialog *progress;
	void DoSomething();
	QFileInfoList progressList;
	QFuture<void> stub;

public slots:
	void onWrite( uint );
	void movementResult(bool);
	void startedStarted(){
		qDebug()<<"Started started!!!11";
	};
};

#endif // DIALOG_H
