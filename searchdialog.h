#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QDir>
#include <QFileInfoList>
#include <QtConcurrent/QtConcurrent>
#include "ui_searchdialog.h"
#include "filefindingsmodel.h"

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
	void show(const QString &startDir);
public slots:
	void on_searchButton_clicked();

private:
	Ui::SearchDialog *ui;
	QStringListModel* model;
	QMutex addBlocker;

	QString updateCombo(CustomDropDown* combo);
	void searchRecursion(QString& pattern, QString& startDir, searchFlags = NAME);
	void addFile(QString& newFile);
};

#endif // SEARCHDIALOG_H
