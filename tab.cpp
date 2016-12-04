#include "tab.h"

#define IN 1
#define OUT 0
#define cout qDebug()

TabbedListView::TabbedListView(QDir directory, QWidget *parent) : QListView(parent){
    this->directory=directory.absolutePath();
    this->setLayoutMode(QListView::Batched);
    model = new QFileSystemModel(this);
    model->setRootPath(this->directory);
    model->setFilter(QDir::AllEntries | QDir::NoDot);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setModel(model);
    setRootIndex(model->index(model->rootPath()));
    connect(model,SIGNAL(directoryLoaded(QString)),this,SLOT(setCurrentSelection(QString)));
    qDebug()<<directory.absolutePath();
}


void TabbedListView::on_doubleClicked(const QModelIndex &index){
    QFileInfo info=model->fileInfo(index);
    if(info.isDir()){
        if(info.fileName().compare(".."))
            chDir(index, IN);
        else
            chDir(index, OUT);
    }else{
		QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
    }
}

void TabbedListView::chDir(const QModelIndex &index, bool in_out){
    qDebug()<<"Dir at input: "<<model->rootPath();
    if(in_out == IN){
        //directory="..";//clever selection
        QDir parentDir(model->fileInfo(index).absoluteFilePath());
        model->setRootPath(parentDir.absolutePath());
        setRootIndex(model->index(model->rootPath()));
    }else{
        QDir parentDir(model->rootPath());
        if(parentDir.isRoot())
            return;
        //directory=parentDir.dirName();
        parentDir.cdUp();
        model->setRootPath(parentDir.absolutePath());
        setRootIndex(model->index(model->rootPath()));
    }
    directory = model->rootDirectory().absolutePath();
    qDebug()<<"Dir at output: "<<model->rootPath() << " directory: "<<directory;
    //setCurrentSelection();
    emit dirChanged(directory/*.dirName()*/, this->index);

}

void TabbedListView::keyPressEvent(QKeyEvent *event){
    //qDebug()<<event->key();
    QModelIndex index;
    //QModelIndexList items = selectionModel()->selectedIndexes();
    if(selectedIndexes().size()>0)
        index = currentIndex();
    else
        index = rootIndex().child(0,0);
    auto key = event->key();
    switch (key) {
    case Qt::Key_Return:
        on_doubleClicked(index);
        break;
    case Qt::Key_Backspace:
        chDir(index, OUT);
        break;
    default:
        QAbstractItemView::keyPressEvent(event);
    }
    qDebug()<<model->fileInfo(currentIndex()).absoluteFilePath();
}

void TabbedListView::init(){
    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_doubleClicked(QModelIndex)));

}

void TabbedListView::setCurrentSelection(QString sel){
    qDebug()<<sel+"/..";
    //QString dotdot("..");
    int rows = model->rowCount(rootIndex());
    QModelIndex ind;
    for(int i=0;i<rows;i++){
        ind = rootIndex().child(i,0);
        qDebug()<< "Index: "<<i<<" filename: " << model->fileInfo(ind).fileName() << " directory: "<<directory;
        if(!directory.compare(model->fileInfo(ind).fileName()))
            break;
    }

    if(ind.isValid())
        setCurrentIndex(ind);

    selectionModel()->select(currentIndex(), QItemSelectionModel::Select);
    qDebug()<<model->fileInfo(currentIndex()).fileName();
}

void TabbedListView::focusInEvent(QFocusEvent *event){
    cout<<"Focus in! "<<event->gotFocus();
    QWidget::focusInEvent(event);
    emit focusEvent(true);
}

void TabbedListView::focusOutEvent(QFocusEvent *event){
    cout<<"Focus out! "<<event->gotFocus();
    QWidget::focusOutEvent(event);
    emit focusEvent(false);
}

QFileInfoList TabbedListView::getSelectedFiles(){
    QFileInfoList selectedFiles;
    QModelIndexList items = selectionModel()->selectedIndexes();
    foreach (auto fileIndex, items) {
        selectedFiles.append(model->fileInfo(fileIndex));
    }
    return selectedFiles;
}
