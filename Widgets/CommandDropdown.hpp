#ifndef DROPDOWN_H
#define DROPDOWN_H

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QKeyEvent>
#include <QProcess>
#include <QStandardItemModel>

class MainWindow;

class CommandDropDown : public QComboBox
{
	Q_OBJECT
public:
	explicit CommandDropDown(QWidget *parent = Q_NULLPTR);
	~CommandDropDown();
	void setMain(MainWindow* mainWindow);
	void readSettings();
signals:
	void cdTo(const QString&) const;
	void focusPreviouslyFocused();
private:
	virtual void keyPressEvent(QKeyEvent *event);
	void processCommand();
	MainWindow *mainWindow;
};

#endif // DROPDOWN_H
