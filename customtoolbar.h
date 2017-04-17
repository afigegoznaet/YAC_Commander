#ifndef CUSTOMTOOLBAR_H
#define CUSTOMTOOLBAR_H

#include <QToolBar>
#include <QPushButton>
#include <QStorageInfo>
#include <QDebug>
#include <QEvent>
#include <QTimer>
//#include <QAction>

class DriveButton : public QPushButton{
	Q_OBJECT
public:
	explicit DriveButton(QString root, QWidget *parent = Q_NULLPTR);
signals:

public slots:
	void click();
protected:
	//virtual bool event(QEvent *e);

private:
	const QString rootPath;
};

class CustomToolbar : public QToolBar{
	Q_OBJECT
public:
	explicit CustomToolbar(QWidget *parent = 0);
	void sendSignal(const QString&) const;

signals:
	void cdTo(const QString&) const;

public slots:
	void update();

private:
	void refreshMountPoints();
	QList<QStorageInfo> volumes;
};

#endif // CUSTOMTOOLBAR_H
