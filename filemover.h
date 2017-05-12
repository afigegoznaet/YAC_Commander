#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <QObject>
#include <QFile>

bool isMovable(QString& from, QString& to);

class FileMover : public QObject
{
	Q_OBJECT
public:
	explicit FileMover(QString source, QString destination, QString action, QObject *parent = 0);
	~FileMover();
	QMetaObject::Connection progress;
	QMetaObject::Connection status;
	//void execute();

signals:
	void bytesProgress(uint);
	void completed(int);


public slots:

private:
	QString destination;
	QString source;
	QString action;
	int copy();
	int move();
};

#endif // FILEMOVER_H
