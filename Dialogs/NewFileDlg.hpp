#ifndef NEWFILE_HPP
#define NEWFILE_HPP


#include <QDialog>

class QLineEdit;
class QCompleter;

class NewFileDlg : public QDialog {
	Q_OBJECT
public:
	explicit NewFileDlg(QString &label, QString &dirName,
						QWidget *parent = nullptr);

	[[nodiscard]] QString getName() const;

protected:
	[[nodiscard]] QLineEdit *getLineEdit() const { return lineEdit; }

private:
	QLineEdit *lineEdit;

signals:

public slots:
};

#endif // NEWFILE_HPP
