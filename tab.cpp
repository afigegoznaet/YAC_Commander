#include "tab.h"

Tab::Tab(QDir directory, QWidget *parent) : QListView(parent), directory(directory){
    model = new QFileSystemModel(this);
    model->setRootPath(directory.absolutePath());
    //model->setFilter(QDir::NoDot);
    setModel(model);
    setRootIndex(model->index(model->rootPath()));
    qDebug()<<directory.absolutePath();
    auto idx = model->index(model->rootPath());
    qDebug() << idx;
    qDebug() <<model->rowCount(idx);
    for (int i = 0 ; i< model->rowCount(idx); i++){
        QModelIndex node = model->index(i,0,idx);
        qDebug() << model->fileName(node);
     }
}
