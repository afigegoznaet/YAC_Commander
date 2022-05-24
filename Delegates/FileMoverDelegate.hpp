#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <QObject>
#include <QFile>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

bool isMovable(QString &from, QString &to);

class FileMoverDelegate : public QObject {
	Q_OBJECT
public:
	explicit FileMoverDelegate(QString source, QString destination,
							   Qt::DropAction action,
							   QObject *	  parent = nullptr);
	~FileMoverDelegate() override;

signals:
	void bytesProgress(uint);
	void completed(int);


public slots:
	void setStatus(int status);

private:
	QString		   source;
	QString		   destination;
	Qt::DropAction action;
	int			   status;
	QWaitCondition cond;

	int copy();
	int move();
};

#endif // FILEMOVER_H
