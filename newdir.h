#ifndef NEWDIR_H
#define NEWDIR_H

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

class NewDir : public QDialog
{
	Q_OBJECT
public:
	explicit NewDir(QWidget *parent = 0);

	QString dirName() const;

private:
	QLineEdit *m_lineEdit;

signals:

public slots:
};



#endif // NEWDIR_H
