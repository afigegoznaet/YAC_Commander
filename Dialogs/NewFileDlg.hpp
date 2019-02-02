#ifndef NEWFILE_HPP
#define NEWFILE_HPP


#include <QDialog>

class QLineEdit;
class QCompleter;

class NewFileDlg : public QDialog {
	Q_OBJECT
public:
	explicit NewFileDlg(QString &label, QString &getName, QWidget *parent = 0);

	QString getName() const;

protected:
	QLineEdit *m_lineEdit;

signals:

public slots:
};

#endif // NEWFILE_HPP
