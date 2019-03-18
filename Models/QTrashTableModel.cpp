#include "QTrashTableModel.hpp"
#include <KDirLister>
#include <QDebug>
#include <QDir>

QTrashTableModel::QTrashTableModel(QObject *parent)
	: QAbstractTableModel(parent) {

	m_trashDirLister = new KDirLister(this);
	connect(m_trashDirLister, SIGNAL(newItems(const KFileItemList &)), this,
			SLOT(addItems(const KFileItemList &)));
	qDebug() << "Open trash"
			 << m_trashDirLister->openUrl(QUrl(QStringLiteral("trash:/")));
	connect(m_trashDirLister, SIGNAL(completed()), this, SIGNAL(completed()));
}

void QTrashTableModel::addItems(const KFileItemList &items) {

	bool wasEmpty = this->items.empty();
	emit beginInsertRows(QModelIndex(), this->items.count() + 1,
						 this->items.count() + items.count());
	for (const auto item : items) {
		this->items.append(item);

		qDebug() << "Name: " << item.name(); /*
		 qDebug() << "Path: " << item.localPath();
		 qDebug() << "URL: " << item.mostLocalUrl();
		 qDebug() << "Text: " << item.text();
		 qDebug() << "URL: " << item.url();
		 qDebug() << "Comment: " << item.comment();
		 qDebug() << "Entry: " << item.entry();
		 qDebug() << "Source URL: "
				  << item.entry().stringValue(KIO::UDSEntry::UDS_EXTRA);
		 qDebug() << "URL: " << item.localPath();
		 qDebug() << "Info: " << item.iconName();
		 */
	}
	emit endInsertRows();
	if (wasEmpty)
		emit trashNotEmpty();
}

QVariant QTrashTableModel::data(const QModelIndex &index, int role) const {

	if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
		const auto &item = items.at(index.row());
		switch (index.column()) {
		case 1:
			return item.mimeComment();
		case 2:
			return QLocale::system().formattedDataSize(
				item.size(), 2, QLocale::DataSizeTraditionalFormat);
		case 3:
			return item.time(KFileItem::ModificationTime);
		default:
			return item.entry().stringValue(KIO::UDSEntry::UDS_EXTRA);
		}
	}
	if (role == Qt::DecorationRole && index.column() == 0) {
		QFileInfo inf(items.at(index.row()).name());
		return icons.icon(inf);
	}
	return {};
}

QVariant QTrashTableModel::headerData(int section, Qt::Orientation orientation,
									  int role) const {
	if (role == Qt::DisplayRole) {
		switch (section) {
		default:
			return QString("Url");
		case 1:
			return QString("Type");
		case 2:
			return QString("Size");
		case 3:
			return QString("Date Modified");
		}
	}

	return {};
}

const KFileItem &QTrashTableModel::itemAt(int index) { return items.at(index); }

void QTrashTableModel::cdTo(const QUrl &url) {
	emit beginRemoveRows(QModelIndex(), 0, items.count() - 1);
	items.clear();
	emit endRemoveRows();
	m_trashDirLister->openUrl(url);
}

KFileItemList QTrashTableModel::getItemsForDir(const QUrl &dir) {
	return m_trashDirLister->itemsForDir(dir, KCoreDirLister::AllItems);
}

KFileItem QTrashTableModel::rootItem() const {
	return m_trashDirLister->rootItem();
}

bool QTrashTableModel::isFinished() { return m_trashDirLister->isFinished(); }

void QTrashTableModel::reload() {
	items.clear();
	m_trashDirLister->openUrl(m_trashDirLister->url(), KDirLister::Reload);
}

void QTrashTableModel::removeRows(const QList<int> &rowsToRemove) {
	for (auto row = rowsToRemove.crbegin(); row != rowsToRemove.crend();
		 ++row) {
		beginRemoveRows(QModelIndex(), *row, *row);
		items.removeAt(*row);
		endRemoveRows();
	}
	if (items.empty())
		emit trashEmpty();
}
