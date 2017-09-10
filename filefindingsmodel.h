#ifndef FILEFINDINGSVIEW_H
#define FILEFINDINGSVIEW_H

#include <QObject>
#include <QAbstractTableModel>

class FileFindingsModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit FileFindingsModel(QObject *parent = Q_NULLPTR);
};

#endif // FILEFINDINGSVIEW_H
