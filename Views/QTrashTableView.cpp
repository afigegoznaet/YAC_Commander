#include "QTrashTableView.hpp"
#include "../Models/QTrashTableModel.hpp"
#include <algorithm>
#include <QHeaderView>
#include <KDirLister>
#include <QGuiApplication>
#include <QLabel>
#include <QMenu>
#include <QDebug>
#include <KIO/EmptyTrashJob>
#include <KIO/RestoreJob>
#include <KIO/DeleteJob>
#include <KJobWidgets>
#include <KJobUiDelegate>

QTrashTableView::QTrashTableView(QWidget *parent) : FileTableView(parent) {

	// this->parent = (qobject_cast<FileTabSelector *>(parent));
	// infoLabel = this->parent->getLabel();

	trashModel = new QTrashTableModel(this);
	menu = new QMenu(this);
	menu->addAction("Restore selected items", this, SLOT(restoreItems()));
	menu->addAction("Purge selected items", this, SLOT(purgeItems()));
	// menu->addAction("Restore selected items", [this] { });
	// menu->addAction("Purge selected items", [this] { });
	menu->addAction("Empty Trash", [] { KIO::emptyTrash(); });

	connect(trashModel, SIGNAL(trashNotEmpty()), this, SIGNAL(trashNotEmpty()));
	connect(trashModel, SIGNAL(trashEmpty()), this, SIGNAL(trashEmpty()));
}

void QTrashTableView::restoreItems() {
	QList<int> rows;
	QList<QUrl> selectedUrls = getSelectedItems(rows);
	KIO::RestoreJob *job = KIO::restoreFromTrash(selectedUrls);
	KJobWidgets::setWindow(job, this);
	job->uiDelegate()->setAutoErrorHandlingEnabled(true);
	connect(job, &KIO::RestoreJob::finished, this,
			[this, rows = std::move(rows)](KJob *job) {
				qDebug() << "Finsihed restorins";
				qDebug() << job->error();
				if (job->error() == KJob::NoError)
					trashModel->removeRows(rows);
			});
}
void QTrashTableView::purgeItems() {
	QList<int> rows;
	QList<QUrl> selectedUrls = getSelectedItems(rows);
	KIO::DeleteJob *job = KIO::del(selectedUrls);
	KJobWidgets::setWindow(job, this);
	job->uiDelegate()->setAutoErrorHandlingEnabled(true);
	connect(job, &KIO::DeleteJob::finished, this,
			[this, rows = std::move(rows)](KJob *job) {
				qDebug() << "Finsihed purging";
				if (job->error() == KJob::NoError)
					trashModel->removeRows(rows);
			});
}

void QTrashTableView::init() {

	setModel(trashModel);

	verticalHeader()->setVisible(false);
	verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader()->setDefaultSectionSize(fontMetrics().height() + 4);

	horizontalHeader()->setStretchLastSection(true);
	horizontalHeader()->setSectionsMovable(true);
	horizontalHeader()->setSectionsClickable(true);
	horizontalHeader()->setSortIndicatorShown(true);


	connect(this, SIGNAL(doubleClicked(const QModelIndex &index)), this,
			SLOT(on_doubleClicked(const QModelIndex &index)));

	setEditTriggers(QAbstractItemView::SelectedClicked);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setTabKeyNavigation(false);

	setSelectionMode(QAbstractItemView::NoSelection);

	// horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this,
			SLOT(headerClicked(int)));

	// delegate = new TableItemDelegate(this);
	// setItemDelegate(delegate);
	// connect(this, SIGNAL(focusEvent(bool)), delegate, SLOT(focused(bool)));
	// connect(horizontalHeader(), &QHeaderView::geometriesChanged, [&]() {
	// itemDelegate()->setRect(horizontalHeader()->geometry()); });
	// connect(selectionModel(), SIGNAL(currentChanged(QModelIndex,
	// QModelIndex)), delegate, SLOT(currentChanged(QModelIndex, QModelIndex)));
	connect(selectionModel(), &QItemSelectionModel::currentChanged, this,
			[this](QModelIndex current, QModelIndex prev) {
				for (int i = 0; i < 4; i++)
					update(current.sibling(current.row(), i));
				for (int i = 0; i < 4; i++)
					update(prev.sibling(prev.row(), i));
			});

	// setDragEnabled(true);
	// setDragDropMode(QAbstractItemView::DragDrop);
	// setDropIndicatorShown(true);

	connect(this, SIGNAL(contextMenuRequested(QPoint)), this,
			SLOT(openContextMenu(QPoint)));

	connect(trashModel, SIGNAL(completed()), this, SLOT(updateInfo()));
}


void QTrashTableView::on_doubleClicked(const QModelIndex &index) {
	const auto &item = trashModel->itemAt(index.row());
	if (item.isDir())
		trashModel->cdTo(item.url());
}

bool QTrashTableView::parseItems(size_t &totalSize, int &dirCount,
								 int &fileCount, const KFileItemList &items) {
	if (items.empty())
		return false;
	for (const auto &item : items) {
		if (item.isDir()) {
			dirCount++;
			KDirLister dirParser;
			dirParser.openUrl(item.url());
			parseItems(totalSize, dirCount, fileCount,
					   trashModel->getItemsForDir(item.url()));
		} else {
			fileCount++;
			totalSize += item.size();
		}
	}
	return true;
}

void QTrashTableView::updateInfo() {

	if (!isCurrent())
		return;

	QString fmt;
	qDebug() << trashModel->isFinished();
	const auto &items =
		trashModel->getItemsForDir(QUrl(QStringLiteral("trash:/")));
	size_t totalSize{};
	int dirCount{};
	int fileCount{};

	bool res = parseItems(totalSize, dirCount, fileCount, items);

	if (!res) {
		fmt += "Trash folder is empty";
		return infoLabel->setText(fmt);
	}

	auto sizeTotal = QLocale::system().formattedDataSize(
		totalSize, 2, QLocale::DataSizeTraditionalFormat);
	auto sizeRemaining = QLocale::system().formattedDataSize(
		totalSize, 2, QLocale::DataSizeTraditionalFormat);


	fmt += sizeTotal + " in " + QString::number(fileCount) + " files, and "
		   + QString::number(dirCount) + " directories in the Trash Bin.";

	infoLabel->setText(fmt);
}

void QTrashTableView::openContextMenu(QPoint) { menu->popup(QCursor::pos()); }

bool QTrashTableView::isCurrent() const {
	return true;
	// return parent->currentWidget() == this;
}

void QTrashTableView::mousePressEvent(QMouseEvent *event) {

	if (event->button() == Qt::RightButton) {

		auto index = indexAt(event->pos());
		if (index.isValid())
			setCurrentIndex(index);
		emit contextMenuRequested(event->pos());
		return;
	}

	setSelectionMode(QAbstractItemView::ExtendedSelection);
	QTableView::mousePressEvent(event);
	setSelectionMode(QAbstractItemView::NoSelection);
}

void QTrashTableView::mouseReleaseEvent(QMouseEvent *event) {
	auto modifiers = qApp->keyboardModifiers();
	if (!(modifiers & (Qt::ControlModifier | Qt::ShiftModifier)))
		clearSelection();
	QTableView::mouseReleaseEvent(event);
}


void QTrashTableView::keyPressEvent(QKeyEvent *event) {
	// QString filter;
	QModelIndex index;
	if (currentIndex().isValid())
		index = currentIndex();
	else
		index = trashModel->index(0, 0, rootIndex());
	const auto &item = trashModel->rootItem();

	auto urlString = item.url().toString();
	auto lastIndex = urlString.lastIndexOf('/', -1);
	auto parentUrl = urlString.mid(0, lastIndex + 1);

	auto key = event->key();
	QFlags<QItemSelectionModel::SelectionFlag> flags =
		QItemSelectionModel::NoUpdate;
	auto modifiers = qApp->keyboardModifiers();
	if (modifiers & Qt::ShiftModifier)
		flags = QItemSelectionModel::Toggle | QItemSelectionModel::Rows;
	switch (key) {
	case Qt::Key_Return:
		on_doubleClicked(index);
		break;
	case Qt::Key_Backspace:
		trashModel->cdTo(parentUrl);
		break;
		/*
	case Qt::Key_Plus:
		setSelectionAction(PLUS);
		break;
	case Qt::Key_Minus:
		setSelectionAction(MINUS);
		break;
	case Qt::Key_Asterisk:
		setSelectionAction(ASTERISK);
		break;
		*/
	case Qt::Key_Right:
	case Qt::Key_Left:
		break;
	case Qt::Key_Space:
		selectionModel()->setCurrentIndex(
			index, QItemSelectionModel::Toggle | QItemSelectionModel::Rows);
		[[fallthrough]];
	case Qt::Key_Down:
		selectionModel()->setCurrentIndex(index, flags);
		index = trashModel->index(index.row() + 1, 0, rootIndex());
		if (index.isValid())
			selectionModel()->setCurrentIndex(index,
											  QItemSelectionModel::NoUpdate);
		break;
	case Qt::Key_Up:
		selectionModel()->setCurrentIndex(index, flags);
		index = trashModel->index(index.row() - 1, 0, rootIndex());
		if (index.isValid())
			selectionModel()->setCurrentIndex(index,
											  QItemSelectionModel::NoUpdate);
		break;
		/*
	case Qt::Key_A:
		if (modifiers == Qt::ControlModifier) {
			selectionModel()->clearSelection();
			setSelectionAction(ASTERISK);
			break;
		}
		[[fallthrough]];
		*/
	default:
		QAbstractItemView::keyPressEvent(event);
		break;
	}
	// qDebug()<<model->fileInfo(currentIndex()).absoluteFilePath();
}

void QTrashTableView::focusInEvent(QFocusEvent *event) {
	QAbstractItemView::focusInEvent(event);
	emit focusEvent(true);
}

void QTrashTableView::focusOutEvent(QFocusEvent *event) {
	QAbstractItemView::focusOutEvent(event);
	emit focusEvent(false);
}

QList<QUrl> QTrashTableView::getSelectedItems(QList<int> &rows) {
	auto indexes = selectionModel()->selectedRows();
	auto items = trashModel->currentItems();
	QList<QUrl> outItems;
	for (const auto idx : indexes) {
		rows.append(idx.row());
		outItems << items.at(idx.row()).url();
	}
	// qSort(rows);
	return outItems;
}
