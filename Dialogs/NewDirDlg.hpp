#ifndef NEWDIR_H
#define NEWDIR_H

#include <QDialog>

class QLineEdit;
class QCompleter;

class NewDir : public QDialog {
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
