#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileInfoList>
#include <QSettings>
#include "ui_progressDialog.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
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
	explicit ProgressDialog(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags() | Qt::Window);
	~ProgressDialog();
	void setFileAction(QFileInfoList fileList, QString destination, ACTION action);

private:
	Ui::ProgressDialog *progress;
	void DoSomething();
	QFileInfoList progressList;
	QFuture<void> stub;
	void DoerSomething(void);
	void dirParsing(QDir& dir, QString &action, QString &destination);
	bool status;

public slots:
	void onWrite( uint );
	void movementResult(bool);
};

#endif // DIALOG_H
