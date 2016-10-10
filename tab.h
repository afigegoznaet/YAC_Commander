#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QListView>
#include <QFileSystemModel>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

class Tab : public QListView
{
	Q_OBJECT
public:
    explicit Tab(QDir diretory, QWidget *parent = 0);
    explicit Tab(QWidget *parent) : Tab(QDir::homePath(),parent){};
    QString GetDirectory(){return directory.dirName();};
signals:


public slots:
private:
    QDir directory;
    QFileSystemModel* model;
};

#endif // TAB_H
