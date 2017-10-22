#ifndef FILEFINDINGSVIEW_H
#define FILEFINDINGSVIEW_H

#include <QObject>
#include <QAbstractTableModel>
#include <QDebug>
#include <QVector>

class FileFindingsModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit FileFindingsModel(QObject *parent = Q_NULLPTR);

	int rowCount(const QModelIndex &) const override;
	int columnCount(const QModelIndex &) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
	int cols = 1;
	QVector<QString> files;
};

#endif // FILEFINDINGSVIEW_H
