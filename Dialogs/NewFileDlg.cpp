#include "NewFileDlg.hpp"

#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QDebug>
#include <QCompleter>
#include <QDirModel>
#include <QFileSystemModel>

NewFileDlg::NewFileDlg(QString &label, QString &dirName, QWidget *parent)
	: QDialog(parent) {

	QLabel *qlabel = new QLabel(label);
	m_lineEdit = new QLineEdit(dirName, this);

	QPushButton *createButton = new QPushButton(tr("Ok"));
	createButton->setDefault(true);

	QPushButton *cancelButton = new QPushButton(tr("Cancel"));

	QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
	buttonBox->addButton(createButton, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout *lt = new QVBoxLayout;
	qlabel->adjustSize();
	lt->addWidget(qlabel);
	lt->addWidget(m_lineEdit);
	lt->addWidget(buttonBox);

	setLayout(lt);
}

QString NewFileDlg::getName() const { return m_lineEdit->text(); }
