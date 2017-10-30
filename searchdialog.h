#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QDir>
#include <QFileInfoList>
#include <QtConcurrent/QtConcurrent>
#include <QQueue>
#include <QRegularExpression>

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

signals:
	void startSearchRecursion(QString pattern, QString startDir);

public slots:
	void on_searchButton_clicked();
	void searchRecursion(QString pattern, QString startDir, searchFlags = NAME);


private:
	Ui::SearchDialog *ui;
	QStringListModel* model;
	QMutex addBlocker;
	QQueue<QString> dirQ;
	QFuture<void> fut;

	QString updateCombo(CustomDropDown* combo);
	void addFile(QString& newFile);
};

#endif // SEARCHDIALOG_H
