#ifndef DROPDOWN_H
#define DROPDOWN_H

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QKeyEvent>

class DropDown : public QComboBox
{
	Q_OBJECT
public:
	explicit DropDown(QWidget *parent = Q_NULLPTR);

private:
	virtual void keyPressEvent(QKeyEvent *event);
};

#endif // DROPDOWN_H
