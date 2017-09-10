#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include "ui_searchdialog.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
	Q_OBJECT
enum searchFlags{NAME, TEXT, DATE_BEFORE=4, DATE_AFTER=8, SIZE_LESS=16, SIZE_MORE=32};
public:
	explicit SearchDialog(QWidget *parent = 0);
	~SearchDialog();
	void startSearch();
	void show(const QString &startDir);
private:
	Ui::SearchDialog *ui;
	QString updateCombo(CustomDropDown* combo);
	void searchRecursion(QString& pattern, QString startDir, searchFlags = NAME);
};

#endif // SEARCHDIALOG_H
