#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QListView>
#include <QFileSystemModel>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QItemSelectionModel>

enum FileMovementAction{COPY, MOVE, SOFTLINK, HARDLINK};

class TabbedListView : public QListView
{
	Q_OBJECT

public:
    explicit TabbedListView(QDir directory, QWidget *parent = 0);
    TabbedListView(QWidget *parent) : TabbedListView(QDir::homePath(),parent){};
    QString GetDirectory(){return directory;};
    void init();
    void setTabOrderIndex(int index){this->index=index;};
    //void setMetaTab(QTabWidget* metaTab){this->metaTab=metaTab;};
signals:
    //void activated(const QModelIndex &index);
    void dirChanged(const QString dirName, int index);
    //void fileMovement(QItemSelectionModel* model, FileMovementAction action);

public slots:
    void on_doubleClicked(const QModelIndex &index);
    void setCurrentSelection(QString sel);
protected:
    virtual void keyPressEvent(QKeyEvent * event);
    void chDir(const QModelIndex &index, bool in_out);
    virtual void focusInEvent(QFocusEvent* event);
    virtual void focusOutEvent(QFocusEvent* event);


private:
    QString directory;
    //QString *selection;
    QFileSystemModel* model;
    int index;
    QTabWidget* metaTab;

};

#endif // TAB_H
