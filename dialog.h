#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileInfoList>
#include <QSettings>

enum ACTION {COPY, MOVE, LN, LN_S, ENUM_TERMINATOR} ;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

	friend class MainWindow;
public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();
	void setFileAction(QFileInfoList fileList, QString destination, ACTION action);

private:
	Ui::Dialog *progress;
	void DoSomething();
	QFileInfoList progressList;
	int writtenKb;

public slots:
	void onWrite( qint64 );
};

#endif // DIALOG_H
