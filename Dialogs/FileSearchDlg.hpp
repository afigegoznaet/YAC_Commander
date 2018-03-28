#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QDir>
#include <QFileInfoList>
#include <QtConcurrent/QtConcurrent>
#include <QQueue>
#include <QRegularExpression>
#include "Widgets/EditableDropdown.hpp"
#include "ui_searchdialog.h"
#include "Models/FileFindingsModel.hpp"
#include <QTime>
#include <QMutex>
#include <functional>

namespace Ui {
class SearchDialog;
}

enum FileAttrib{
	X = 1, W = 2, R = 4, D = 8
};
enum SearchToggles{
	TextPattern = 1, Date = 2, Size = 4, Attributes = 8
};

enum SizeOp{
	EQ, G, L
};

enum SizeMod{
	Byte, KByte, MByte, GByte
};

struct SearchAttrib{
	QString pattern;
	quint8 attrFlags;
	quint8 togglesFlags;
	QDateTime startDate;
	QDateTime endDate;
	SizeOp op;
	quint64 size;
};

class MainWindow;

class SearchDialog : public QDialog
{
	Q_OBJECT
enum searchFlags{NAME, TEXT, DATE_BEFORE=4, DATE_AFTER=8, SIZE_LESS=16, SIZE_MORE=32};
public:
	explicit SearchDialog(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags() | Qt::Window);
	~SearchDialog();
	void show(const QString &startDir);

signals:
	void startSearchRecursion(QString pattern, QString startDir);
	void rowsInserted(const QModelIndex &parent, int first, int last);

public slots:
	void on_searchButton_clicked();
	void searchRecursion(QString pattern, QString startDir, searchFlags = NAME);
	void on_doubleClicked(const QModelIndex &index);

private slots:
	void on_clearButton_clicked();
	void on_dateCheck_toggled(bool checked);
	void on_sizeCheck_toggled(bool checked);
	void on_attributesCheck_toggled(bool checked);

	void on_caseCheckBox_stateChanged(int checkState);

	void on_regExpCheckBox_stateChanged(int checkState);

private:
	Ui::SearchDialog *ui;
	QStringListModel* model;
	QMutex addBlocker;
	QQueue<QString> dirQ;
	QFuture<void> fut;
	bool searching;
	MainWindow* parentWindow;
	int firstRow;
	SearchAttrib attrs;
	QTime searchTime;
	//QMutex locker[4];
	QMutex dirListLocker;

	std::atomic_int counter;

	QString updateCombo(EditableDropDown* combo);
	void addFile(const QString &newFile);
	void validateFile(QFileInfo& theFile);
	void resetGuiState();
	//void paintEvent(QPaintEvent *event) override;

};

#endif // SEARCHDIALOG_H
