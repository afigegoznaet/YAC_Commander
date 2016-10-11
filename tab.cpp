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


void Tab::on_doubleClicked(const QModelIndex &index){
    QFileInfo info=model->fileInfo(index);
    model->setRootPath(info.absoluteFilePath());
    setRootIndex(model->index(model->rootPath()));
}

void Tab::keyPressEvent(QKeyEvent *event){
    qDebug()<<event->key();


}

void Tab::init(){
    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_doubleClicked(QModelIndex)));
}
