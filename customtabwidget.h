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
	~CustomTabWidget();
	TabbedListView *addNewTab(bool dup=false, QString dir="");
	void readSettings();

public slots:
	void onDirChanged(const QString dirName, int tabIndex);
	void onFocusEvent(bool);
	void sectionResized(int logicalIndex, int oldSize, int newSize);

signals:
	void gotResized(int logicalIndex, int oldSize, int newSize);

protected:
	QString defaultStyle;

private:
	virtual void mousePressEvent(QMouseEvent *event);

};

#endif // CUSTOMTABWIDGET_H
