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
	explicit FileMover(QFile &from, QString &destination, QString &action, QObject *parent = 0);
	~FileMover();
	void start();

signals:
	void bytesMoved(quint64);
	void completed(bool);

public slots:

private:
	QString& destination;
	QFile &from;
	QString &action;
	std::future<bool> result;
};

#endif // FILEMOVER_H
