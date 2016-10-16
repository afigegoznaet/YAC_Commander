#include "tab.h"

#define IN 1
#define OUT 0

Tab::Tab(QDir directory, QWidget *parent) : QListView(parent), directory(directory){
    this->setLayoutMode(QListView::Batched);
    model = new QFileSystemModel(this);
    model->setRootPath(directory.absolutePath());
    //model->setFilter(QDir::NoDot);
    setModel(model);
    setRootIndex(model->index(model->rootPath()));
    qDebug()<<directory.absolutePath();
}


void Tab::on_doubleClicked(const QModelIndex &index){
    QFileInfo info=model->fileInfo(index);
    if(info.isDir()){
        chDir(index, IN);
    }else{
        QDesktopServices::openUrl(QUrl(info.absoluteFilePath()));
    }
}

void Tab::chDir(const QModelIndex &index, bool in_out){
    if(in_out == IN){
        model->setRootPath(model->fileInfo(index).absoluteFilePath());
        setRootIndex(model->index(model->rootPath()));
    }else{
        QDir parentDir(model->fileInfo(index).dir());
        parentDir.cdUp();
        model->setRootPath(parentDir.absolutePath());
        setRootIndex(model->index(model->rootPath()));
    }
    QModelIndex ind = rootIndex().child(0,0);
    if(ind.isValid())
        setCurrentIndex(ind);
    selectionModel()->select(currentIndex(), QItemSelectionModel::Select);
    qDebug()<<model->fileInfo(currentIndex()).fileName();

}

void Tab::keyPressEvent(QKeyEvent *event){
    //qDebug()<<event->key();
    QModelIndex index;
    if(selectedIndexes().size()>0)
        index = *selectedIndexes().begin();
    else
        index = currentIndex();
    QFileInfo info;
    info=model->fileInfo(index);
    int count = model->rowCount(model->parent(currentIndex()));
    auto key = event->key();
    switch (key) {
    case Qt::Key_Return:
        if(info.isDir()){
            chDir(index, IN);
        }else{
            QDesktopServices::openUrl(QUrl(info.absoluteFilePath()));
        }
        break;
    case Qt::Key_Backspace:
        chDir(index, OUT);
        break;
    case Qt::Key_Up:
        if(index.row()>0)
            setCurrentIndex(index.sibling(index.row()-1,0));
        break;
    case Qt::Key_Down:
        if(index.row()<count-1)
            setCurrentIndex(index.sibling(index.row()+1,0));
        break;
    default:
        break;
    }
    qDebug()<<model->fileInfo(currentIndex()).fileName();
}

void Tab::init(){
    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_doubleClicked(QModelIndex)));
}
