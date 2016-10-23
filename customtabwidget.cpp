#include "customtabwidget.h"

CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent) {}

void CustomTabWidget::onDirChanged(const QString dirName, int tabIndex){
    this->setTabText(tabIndex, dirName);
    qDebug()<<this->hasFocus();
    qDebug()<<this->isActiveWindow();
    qDebug()<<this->isEnabled();
}
