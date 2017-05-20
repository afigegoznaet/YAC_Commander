#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileInfoList>
#include <QSettings>
#include <QPushButton>
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
	void DoSomething(void);
	QFileInfoList progressList;
	QFuture<void> stub;
	//void DoerSomething(void);
	void dirParsing(QDir& dir, QString &action, QString &destination);
	bool status;
	QWaitCondition cond;
	QWaitCondition condStatus;
	void switchText();
	QString pauseButtonLabels[2]= {"Continue","Pause"};

signals:
	void sendErrMsg(QString errorText);
	void hideDialogSignal();
	void dirMoved(int);
	void setStatus(int status);

public slots:
	void onWrite( uint );
	void movementResult(int);
	void dirMovementResult(int);
	void errorMsg(QString errorText);
	void hideDialogSlot();
private slots:
	void on_pauseButton_clicked();
	void on_removeButton_clicked();
	void on_abortButton_clicked();
};

#endif // DIALOG_H
