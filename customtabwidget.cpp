#include "customtabwidget.h"

CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent) {
    defaultStyle = styleSheet();
}

void CustomTabWidget::onDirChanged(const QString dirName, int tabIndex){
    setTabText(tabIndex, dirName);
    tabBar()->setDrawBase(true);
    tabBar()->setExpanding(true);
    tabBar()->setMovable(true);
    qDebug()<<this->isActiveWindow();
    qDebug()<<this->isEnabled();
}

void CustomTabWidget::onFocusEvent(bool focused){
    if(focused)
        setStyleSheet("border: 1px solid green");
    else
        setStyleSheet(defaultStyle);
}
