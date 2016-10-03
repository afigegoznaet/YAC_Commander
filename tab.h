#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QListView>

class Tab : public QListView
{
	Q_OBJECT
public:
	explicit Tab(QString diretory, QWidget *parent = 0);

signals:

public slots:
private:
	QString directory;
};

#endif // TAB_H
