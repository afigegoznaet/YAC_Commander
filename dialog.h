#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileInfoList>

enum ACTION {COPY, MOVE, LN, LN_S, ENUM_TERMINATOR} ;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();
	void setFileAction(QFileInfoList fileList, QString destination, ACTION action);

private:
	Ui::Dialog *progress;
};

#endif // DIALOG_H
