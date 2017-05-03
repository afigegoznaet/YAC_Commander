#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <QObject>
#include <QFile>

bool isMovable(QString& from, QString& to);

class FileMover : public QObject
{
	Q_OBJECT
public:
	explicit FileMover(QString from, QString destination, QString action, QObject *parent = 0);
	~FileMover();
	QMetaObject::Connection progress;
	QMetaObject::Connection status;
	//void execute();

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
