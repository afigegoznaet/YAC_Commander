#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileInfoList>
#include <QSettings>

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
	int writtenKb;

public slots:
	void onWrite( qint64 );
};

#endif // DIALOG_H
