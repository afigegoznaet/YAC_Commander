#ifndef NEWDIR_H
#define NEWDIR_H

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QCompleter>
#include <QDirModel>

class NewDir : public QDialog
{
	Q_OBJECT
public:
	explicit NewDir(QString &label, QString &dirName, QWidget *parent = 0);

	QString dirName() const;

private:
	QLineEdit *m_lineEdit;
	QCompleter *dirCompleter;

signals:

public slots:
};



#endif // NEWDIR_H
