#ifndef FILEFINDINGSMODEL_H
#define FILEFINDINGSMODEL_H

#include <QAbstractListModel>
#include <QVector>

class FileFindingsModel : public QAbstractListModel {
	Q_OBJECT
public:
	explicit FileFindingsModel(QObject *parent = Q_NULLPTR);

	void addItem(const QString &newItem);
	int rowCount(const QModelIndex &parent) const override;
	// bool insertRows(int row, int count, const QModelIndex &parent =
	// QModelIndex()) override;
	QVariant data(const QModelIndex &index, int role) const override;
	// bool insertRow(int row, const QModelIndex &parent = QModelIndex())
	// override;
private:
	QVector<QString> files;
};

#endif // FILEFINDINGSMODEL_H
