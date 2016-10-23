#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QDebug>

class CustomTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    CustomTabWidget(QWidget* parent = Q_NULLPTR);

public slots:
    void onDirChanged(const QString dirName, int tabIndex);
};

#endif // CUSTOMTABWIDGET_H
