#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <QObject>
#include <QFile>
#include <future>
#include <tuple>

class FileMover : public QObject
{
	Q_OBJECT
public:
	explicit FileMover(QString from, QString destination, QString action, QObject *parent = 0);
	~FileMover();
	QMetaObject::Connection progress;
	QMetaObject::Connection status;

signals:
	void bytesProgress(uint);
	void completed(bool);


public slots:

private:
	QString destination;
	QString from;
	QString action;
	bool copy();
	bool move();
	void DoSomething();
};

#endif // FILEMOVER_H
