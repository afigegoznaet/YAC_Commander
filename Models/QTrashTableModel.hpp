#ifndef QTRASHMODEL_HPP
#define QTRASHMODEL_HPP

#include <QAbstractTableModel>
#include <KFileItem>
#include <QFileIconProvider>

class KDirLister;
class QFileIconProvider;
class QTrashTableModel : public QAbstractTableModel {
	Q_OBJECT
public:
	explicit QTrashTableModel(QObject *parent = nullptr);
	int rowCount(const QModelIndex &parent = QModelIndex()) const override {
		Q_UNUSED(parent);
		return items.count();
	}
	int columnCount(const QModelIndex &parent = QModelIndex()) const override {
		Q_UNUSED(parent);
		return 4;
	}
	QVariant data(const QModelIndex &index,
				  int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const override;
	const KFileItem &itemAt(int index);
	KFileItem rootItem() const;
	KFileItemList getItemsForDir(const QUrl &dir);
	KFileItemList currentItems() { return items; }
	bool isFinished();
	void cdTo(const QUrl &url);

signals:
	void completed();
	void trashNotEmpty();
	void trashEmpty();
public slots:
	void addItems(const KFileItemList &items);
	void reload();
	void removeRows(const QList<int> &rowsToRemove);

protected:
private:
	KFileItemList items;
	KDirLister *m_trashDirLister;
	QFileIconProvider icons;
};

#endif // QTRASHMODEL_HPP
