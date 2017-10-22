#include "searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SearchDialog)
{
	ui->setupUi(this);
	ui->fileMaskcombo->setParentDlg(this);
	ui->fileMaskcombo->setEditable(true);
	ui->startDirCombo->setParentDlg(this);
	ui->startDirCombo->setEditable(true);

	model = new FileFindingsModel(this);
}

SearchDialog::~SearchDialog()
{
	delete ui;
}

void SearchDialog::show(const QString &startDir){
	ui->fileMaskcombo->lineEdit()->setText("*.*");
	ui->startDirCombo->lineEdit()->setText(startDir);
	QDialog::show();
}

void SearchDialog::startSearch(){
	QString fileMask = updateCombo(ui->fileMaskcombo);
	QString startDir = updateCombo(ui->startDirCombo);

}

QString SearchDialog::updateCombo(CustomDropDown* combo){
	QString text(combo->lineEdit()->text());
	if(combo->findText(text)<0)
		combo->insertItem(0,text);
	return text;
}

void SearchDialog::searchRecursion(QString& pattern, QString startDir, searchFlags){

}
