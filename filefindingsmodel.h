#ifndef FILEFINDINGSVIEW_H
#define FILEFINDINGSVIEW_H

#include <QObject>
#include <QAbstractListModel>
#include <QDebug>
#include <QVector>

class FileFindingsModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit FileFindingsModel(QObject *parent = Q_NULLPTR);

	int rowCount(const QModelIndex &) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:

	QVector<QString> files;
};

#endif // FILEFINDINGSVIEW_H
