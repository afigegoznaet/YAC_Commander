#ifndef DROPDOWN_H
#define DROPDOWN_H

#include <QComboBox>


class MainWindow;

class CommandDropDown : public QComboBox {
	Q_OBJECT
public:
	explicit CommandDropDown(QWidget *parent = Q_NULLPTR);
	~CommandDropDown() override;
	void setMain(MainWindow *mainWindow);
	void readSettings();
signals:
	void cdTo(const QString &) const;
	void focusPreviouslyFocused();

private:
	void		keyPressEvent(QKeyEvent *event) override;
	void		processCommand();
	MainWindow *mainWindow{};
};

#endif // DROPDOWN_H
