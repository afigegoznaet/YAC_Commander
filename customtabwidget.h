#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QDebug>
#include <QTabBar>


class CustomTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    CustomTabWidget(QWidget* parent = Q_NULLPTR);

public slots:
    void onDirChanged(const QString dirName, int tabIndex);
    void onFocusEvent(bool);

protected:
    //void paintEvent(QPaintEvent *event);
    QString defaultStyle;
};

#endif // CUSTOMTABWIDGET_H
