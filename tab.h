#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QListView>
#include <QFileSystemModel>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QKeyEvent>

class Tab : public QListView
{
	Q_OBJECT
public:
    explicit Tab(QDir diretory, QWidget *parent = 0);
    Tab(QWidget *parent) : Tab(QDir::homePath(),parent){};
    QString GetDirectory(){return directory.dirName();};
    void init();
signals:
    void activated(const QModelIndex &index);

public slots:
    void on_doubleClicked(const QModelIndex &index);
protected:
    virtual void keyPressEvent(QKeyEvent * event);

private:
    QDir directory;
    QFileSystemModel* model;
};

#endif // TAB_H
