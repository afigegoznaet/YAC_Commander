#ifndef STATUSCHECKER_H
#define STATUSCHECKER_H

#include <QObject>

class StatusChecker : public QObject
{
	Q_OBJECT
public:
	explicit StatusChecker(QObject *parent = 0);

signals:

public slots:
};

#endif // STATUSCHECKER_H