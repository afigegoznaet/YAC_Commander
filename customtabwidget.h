#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QDebug>
#include <QTabBar>
#include <QMenu>
#include <QMouseEvent>
#include "tab.h"



class CustomTabWidget : public QTabWidget
{
	Q_OBJECT
public:
	CustomTabWidget(QWidget* parent = Q_NULLPTR);

public slots:
	void onDirChanged(const QString dirName, int tabIndex);
	void onFocusEvent(bool);

protected:
	QString defaultStyle;
private:
	virtual void mousePressEvent(QMouseEvent *event);
	void addNewTab(bool dup=false);
};

#endif // CUSTOMTABWIDGET_H
