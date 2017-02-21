#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <QObject>
#include <QFile>

class FileMover : public QObject
{
	Q_OBJECT
public:
	explicit FileMover(QFile &from, QString &destination, QString &action, QObject *parent = 0);

signals:
	void bytesMoved(quint64);

public slots:

private:
	//bool fileMovement(std::tuple<>);
};

#endif // FILEMOVER_H
