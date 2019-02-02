#include "NewDirDlg.hpp"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QCompleter>
#include <QDirModel>

NewDirDlg::NewDirDlg(QString &label, QString &dirName, QWidget *parent)
	: NewFileDlg(label, dirName, parent) {

	fileCompleter = new QCompleter(this);
	auto dm = new QDirModel(fileCompleter);
	fileCompleter->setModel(dm);
	fileCompleter->setCompletionMode(QCompleter::PopupCompletion);
	if (!dirName.isEmpty())
		m_lineEdit->setCompleter(fileCompleter);
}
