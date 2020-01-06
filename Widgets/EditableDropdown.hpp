#ifndef CUSTOMDROPDOWN_H
#define CUSTOMDROPDOWN_H

#include <QComboBox>

class SearchDialog;

class EditableDropDown : public QComboBox {
	Q_OBJECT
public:
	EditableDropDown(QWidget *parent);
	void setParentDlg(QDialog *parentDlg) { this->parentDlg = parentDlg; }

private:
	virtual void keyPressEvent(QKeyEvent *event) final;
	virtual void processCommand();
	QDialog *	parentDlg = nullptr;

	friend class SearchDialog;
};

#endif // CUSTOMDROPDOWN_H
