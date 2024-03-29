#include "Views/FileTableView.hpp"
#include "MainWindow.hpp"
#include "Widgets/FileTabSelector.hpp"
#include <QMessageBox>

#include "Delegates/TableItemDelegate.hpp"
#include "Menus/ItemContextMenu.hpp"
#include "Models/OrderedFileSystemModel.hpp"
#include <QDebug>
#include <QDesktopServices>
#include <QFileSystemModel>
#include <QGuiApplication>
#include <QHeaderView>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QKeyEvent>
#include <QLabel>
#include <QSettings>
#include <QStorageInfo>
#include <QTimer>
#include <QUrl>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

static constexpr auto IN = 1;
static constexpr auto OUT = 0;

FileTableView::FileTableView(QDir &&directory, QWidget *parent)
	: QTableView(parent), directory(directory.absolutePath()),
	  slowDoubleClickTimer(this), dirWatch(this) {
	while (!directory.exists()) {
		if (!directory.cdUp())
			directory = QDir::home();
	}
	this->directory = directory.absolutePath();

	connect(&slowDoubleClickTimer, &QTimer::timeout, this,
			[&] { slowDoubleClick = false; });
	this->parent = (qobject_cast<FileTabSelector *>(parent));
	infoLabel = this->parent->getLabel();
	menu = new ItemContextMenu(this);
	model = new OrderedFileSystemModel(this);
	prevRow = -1;
	dirWatch.addPath(directory.absolutePath());
	connect(&dirWatch, SIGNAL(directoryChanged(const QString &)), this, SLOT(fileWatcher(const QString &)));
	connect(&dirWatch, SIGNAL(fileChanged(const QString &)), this, SLOT(fileWatcher(const QString &)));
}

FileTableView::~FileTableView() {
	assert(true);
}

void FileTableView::fileWatcher(const QString &) {
	// qDebug() << "File changed \n" << file;
	auto res = model->setRootPath(model->rootPath());
	qDebug() << res;
}

void FileTableView::on_doubleClicked(const QModelIndex &index) {
	QFileInfo info = model->fileInfo(index); // mapped
	if (info.isDir()) {
		if (info.fileName().compare(".."))
			chDir(index, IN);
		else
			chDir(index, OUT);
	} else {
#ifdef TEST_EXEC

		if (info.isExecutable()) {
			QProcess	proc;
			QString		program;
			QStringList args;
			args << "-exec";
			args.append(info.absoluteFilePath());
			program = "sh";

			proc.startDetached(program, args, directory);

		} else

#endif

			QDesktopServices::openUrl(
				QUrl::fromLocalFile(info.absoluteFilePath()));
	}
}

void FileTableView::chDir(const QModelIndex &index, bool in_out) {
	prevRow = -1;
	dirWatch.removePath(model->rootPath());
	if (in_out == IN) {
		directory = ".."; // clever selection
		QDir parentDir(model->fileInfo(index).absoluteFilePath());
		auto rootIndex = model->setRootPath(parentDir.absolutePath());
		assert(rootIndex.isValid());
		parentDir.cd(".");
		setRootIndex(model->getRootIndex());
		dirWatch.addPath(parentDir.absolutePath());
	} else {
		QDir parentDir(model->rootPath());
		if (parentDir.isRoot())
			return;
		directory = parentDir.dirName();
		parentDir.cdUp();
		setRootIndex(model->setRootPath(parentDir.absolutePath()));
		dirWatch.addPath(parentDir.absolutePath());
	}

	selectionModel()->clear();
	updateInfo();
	emit dirChanged(model->rootPath(), this->index);
}

void FileTableView::keyPressEvent(QKeyEvent *event) {
	// QString filter;
	QModelIndex index;
	if (currentIndex().isValid())
		index = currentIndex();
	else
		index = model->index(0, 0, rootIndex()); // rootIndex().child(0, 0);
	auto									   key = event->key();
	QFlags<QItemSelectionModel::SelectionFlag> flags =
		QItemSelectionModel::NoUpdate;
	auto modifiers = qApp->keyboardModifiers();
	if (modifiers & Qt::ShiftModifier)
		flags = QItemSelectionModel::Toggle | QItemSelectionModel::Rows;
	switch (key) {
	case Qt::Key_Return:
		if (editorIsOpen) {
			editorIsOpen = false;
			return QAbstractItemView::keyPressEvent(event);
		}
		on_doubleClicked(index);
		break;
	case Qt::Key_Backspace:
		chDir(index, OUT);
		break;
	case Qt::Key_Plus:
		setSelectionAction(PLUS);
		break;
	case Qt::Key_Minus:
		setSelectionAction(MINUS);
		break;
	case Qt::Key_Asterisk:
		setSelectionAction(ASTERISK);
		break;
	case Qt::Key_Right:
	case Qt::Key_Left:
		break;
	case Qt::Key_Space:
		selectionModel()->setCurrentIndex(
			index, QItemSelectionModel::Toggle | QItemSelectionModel::Rows);
		[[fallthrough]];
	case Qt::Key_Down:
		selectionModel()->setCurrentIndex(index, flags);
		index = model->index(index.row() + 1, 0, rootIndex());
		// index = rootIndex().child(index.row() + 1, 0);
		if (index.isValid())
			selectionModel()->setCurrentIndex(index,
											  QItemSelectionModel::NoUpdate);
		break;
	case Qt::Key_Up:
		selectionModel()->setCurrentIndex(index, flags);
		index = model->index(index.row() - 1, 0, rootIndex());
		// index = rootIndex().child(index.row() - 1, 0);
		if (index.isValid())
			selectionModel()->setCurrentIndex(index,
											  QItemSelectionModel::NoUpdate);
		break;
	case Qt::Key_A:
		if (modifiers == Qt::ControlModifier) {
			selectionModel()->clearSelection();
			setSelectionAction(ASTERISK);
			break;
		}
		[[fallthrough]];
	default:
		QAbstractItemView::keyPressEvent(event);
		break;
	}
	// qDebug()<<model->fileInfo(currentIndex()).absoluteFilePath();
}

void FileTableView::init() {

	setEditTriggers(QAbstractItemView::SelectedClicked
					| QAbstractItemView::EditKeyPressed);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setTabKeyNavigation(false);
	horizontalHeader()->setStretchLastSection(true);
	horizontalHeader()->setSectionsMovable(true);
	horizontalHeader()->setSectionsClickable(true);
	horizontalHeader()->setSortIndicatorShown(true);

	verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader()->setDefaultSectionSize(fontMetrics().height() + 4);

	auto fModel = new QFileSystemModel(this);
	model->setSourceModel(fModel);
	auto modelRootIndex = model->setRootPath(this->directory);
	assert(modelRootIndex.isValid());
	auto topWidgets = QApplication::topLevelWidgets();

	auto filters = QDir::AllEntries | QDir::NoDot | QDir::System;

	for (auto widget : topWidgets) {
		auto topWidget = qobject_cast<MainWindow *>(widget);
		if (topWidget) {
			if (topWidget->showHidden())
				filters |= QDir::Hidden;
			break;
		}
	}

	model->setFilter(filters);
	setSelectionMode(QAbstractItemView::NoSelection);
	setModel(model);
	model->setFilterRegExp("");
	setRootIndex(modelRootIndex);
	verticalHeader()->setVisible(false);

	connect(this, SIGNAL(doubleClicked(QModelIndex)), this,
			SLOT(on_doubleClicked(QModelIndex)));
	// horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this,
			SLOT(headerClicked(int)));
	selectionModel()->select(model->index(1, 0, rootIndex()),
							 QItemSelectionModel::Current);

	// this is needed for clever file selection whn moving up and down
	connect(fModel, &QFileSystemModel::directoryLoaded, [&]() {
		prevRow = model->mapToSource(currentIndex()).row();
		model->sort();
	});
	connect(model, SIGNAL(directoryLoaded(QString)), this,
			SLOT(setCurrentSelection(QString)));
	connect(model, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), this,
			SLOT(rowsRemoved(QModelIndex, int, int)));

	connect(selectionModel(), &QItemSelectionModel::selectionChanged, this,
			&FileTableView::updateInfo);

	delegate = new TableItemDelegate(this);
	setItemDelegate(delegate);
	connect(this, SIGNAL(focusEvent(bool)), delegate, SLOT(focused(bool)));
	connect(horizontalHeader(), &QHeaderView::geometriesChanged,
			[&]() { itemDelegate()->setRect(horizontalHeader()->geometry()); });
	connect(selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
			delegate, SLOT(currentChanged(QModelIndex, QModelIndex)));
	connect(selectionModel(), &QItemSelectionModel::currentChanged, this,
			[this](QModelIndex current, QModelIndex prev) {
				for (int i = 0; i < 4; i++)
					update(current.sibling(current.row(), i));
				for (int i = 0; i < 4; i++)
					update(prev.sibling(prev.row(), i));
			});

	connect(model,
			SIGNAL(setFileAction(QFileInfoList, QString, Qt::DropAction)), this,
			SIGNAL(setFileAction(QFileInfoList, QString, Qt::DropAction)));
	setDragEnabled(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDropIndicatorShown(true);

	connect(this, SIGNAL(contextMenuRequested(QPoint)), this,
			SLOT(openContextMenu(QPoint)));

	connect(delegate, SIGNAL(commitData(QWidget *)), this,
			SLOT(commitNewName(QWidget *)));
}

void FileTableView::setCurrentSelection(const QString &) {
	if (prevRow > 0) {
		/*auto ind =
				model->mapFromSource(model->getSourceRootIndex().child(prevRow,
		0)); setCurrentIndex(ind);
	*/
		selectionModel()->currentChanged(currentIndex(), currentIndex());

		// scrollTo(currentIndex());
		return;
	}
	int	 rows = model->rowCount(rootIndex());
	auto rootIndex = model->getRootIndex();
	auto index = model->index(0, 0, rootIndex);
	int	 i;
	for (i = 0; i < rows; i++) {
		auto ind = model->index(i, 0, rootIndex);
		if (!directory.compare(model->fileInfo(ind).fileName())) {
			index = ind;
			break;
		}
	}

	selectionModel()->setCurrentIndex(index, QItemSelectionModel::NoUpdate);
	prevRow = model->mapToSource(index).row();
	scrollTo(index);
}

void FileTableView::headerClicked(int section) {

	prevRow = model->mapToSource(currentIndex()).row();
	Qt::SortOrder order = Qt::AscendingOrder;
	if (section == horizontalHeader()->sortIndicatorSection())
		if (Qt::AscendingOrder == horizontalHeader()->sortIndicatorOrder())
			order = Qt::DescendingOrder;
	model->sort(section, order);
}

void FileTableView::focusInEvent(QFocusEvent *event) {
	QAbstractItemView::focusInEvent(event);
	emit focusEvent(true);
	updateInfo();
}

void FileTableView::focusOutEvent(QFocusEvent *event) {
	QAbstractItemView::focusOutEvent(event);
	emit focusEvent(false);
}

QFileInfoList FileTableView::getSelectedFiles() {
	QFileInfoList	selectedFiles;
	QModelIndexList items = getSelectedIndexes();

	for (const auto &fileIndex : items)
		selectedFiles.append(model->fileInfo(fileIndex));

	return selectedFiles;
}

QModelIndexList FileTableView::getSelectedIndexes() {
	QModelIndexList items = selectionModel()->selectedRows();
	auto			currIdx = model->fileInfo(selectionModel()->currentIndex());
	if (items.empty() && currIdx.fileName().compare(".."))
		items.append(selectionModel()->currentIndex());
	return items;
}

void FileTableView::cdTo(const QString &dir) {
	auto rootIndex = model->setRootPath(dir);
	assert(rootIndex.isValid());
	setRootIndex(rootIndex);
	emit dirChanged(dir, this->index);
	updateInfo();
}

[[nodiscard]] TableItemDelegate *FileTableView::itemDelegate() const {
	return qobject_cast<TableItemDelegate *>(QTableView::itemDelegate());
}

void FileTableView::mousePressEvent(QMouseEvent *event) {

	if (event->button() == Qt::RightButton) {

		auto index = indexAt(event->pos());
		if (index.isValid())
			setCurrentIndex(index);
		emit contextMenuRequested(event->pos());
		return;
	}

	auto index = indexAt(event->pos());
	if (slowDoubleClick && currentIndex() == index)
		return openEditor(index);

	slowDoubleClickTimer.start(1000);
	slowDoubleClick = true;

	setSelectionMode(QAbstractItemView::ExtendedSelection);
	QTableView::mousePressEvent(event);
	setSelectionMode(QAbstractItemView::NoSelection);
}

void FileTableView::openEditor(QModelIndex &index) {
	if (index.row() > 0)
		editorIsOpen = edit(index, QAbstractItemView::AllEditTriggers, nullptr);
}

void FileTableView::mouseReleaseEvent(QMouseEvent *event) {
	auto modifiers = qApp->keyboardModifiers();
	if (!(modifiers & (Qt::ControlModifier | Qt::ShiftModifier)))
		clearSelection();
	QTableView::mouseReleaseEvent(event);
}

void FileTableView::queryDialog(QString &filter, Action act) {
	bool	ok;
	QString label;
	switch (act) {
	case PLUS:
		label = "Select";
		break;
	case MINUS:
		label = "Deselect";
		break;
	default:
		return;
	}
	filter = QInputDialog::getText(this, "Filter selection", label,
								   QLineEdit::Normal, "*.*", &ok);
}

void FileTableView::setSelectionAction(Action act) {
	QString							   filter("");
	int								   rowCount = model->rowCount(rootIndex());
	QItemSelectionModel::SelectionFlag selectionType;
	switch (act) {
	case PLUS:
		// this->clearSelection();
		selectionType = QItemSelectionModel::Select;
		queryDialog(filter, PLUS);
		break;
	case MINUS:
		selectionType = QItemSelectionModel::Deselect;
		queryDialog(filter, MINUS);
		break;
	default:
		selectionType = QItemSelectionModel::Toggle;
		break;
	}

	// QMutex selMutex;
	//	QRegularExpression reg(
	//		filter, QRegularExpression::DontCaptureOption
	//					|
	// QRegularExpression::OptimizeOnFirstUsageOption);
	QRegExp reg(filter, Qt::CaseSensitive, QRegExp::Wildcard);
	{
		const QSignalBlocker blocker(selectionModel());
		for (int i = 1; i < rowCount; i++) {

			auto ind = model->index(i, 0, rootIndex());
			if (filter.isEmpty()) {
				this->selectionModel()->select(
					ind, selectionType | QItemSelectionModel::Rows);
			} else {
				auto fName = model->fileInfo(ind).fileName();
				// auto m = reg.match(fName);
				if (fName.contains(reg)) {
					this->selectionModel()->select(
						ind, selectionType | QItemSelectionModel::Rows);
				}
			}
		}
	}
	emit selectionModel()->selectionChanged(selectionModel()->selection(),
											QItemSelection());
	repaint();
}

void FileTableView::rowsRemoved(const QModelIndex &idx, int first, int last) {

	if (model->rowCount(idx) > last + 1) {
		selectionModel()->setCurrentIndex(model->index(last + 1, 0, idx), QItemSelectionModel::Current | QItemSelectionModel::Rows);
	} else {
		selectionModel()->setCurrentIndex(model->index(first - 1, 0, idx), QItemSelectionModel::Current | QItemSelectionModel::Rows);
	}

	delegate->currentChanged(currentIndex(), currentIndex());
	updateInfo();
}

void FileTableView::rowsInserted(const QModelIndex &parent, int first,
								 int last) {
	Q_UNUSED(parent)
	Q_UNUSED(first)
	Q_UNUSED(last)

	updateInfo();
}

void FileTableView::updateInfo() {

	if (!isCurrent())
		return;
	QStorageInfo storage(model->rootPath());
	// qDebug()<<"Root path:"<< storage.rootPath();
	QString fmt;
	auto	sizeTotal = QLocale::system().formattedDataSize(
		   storage.bytesTotal(), 2, QLocale::DataSizeTraditionalFormat);
	auto sizeRemaining = QLocale::system().formattedDataSize(
		storage.bytesAvailable(), 2, QLocale::DataSizeTraditionalFormat);

	fmt += model->rootPath();

	fmt += "\n";
	fmt += sizeRemaining + " available of " + sizeTotal;
	fmt += "\t" + QString::number(selectionModel()->selectedRows().size())
		   + " selected of " + QString::number(model->rowCount(rootIndex()) - 1)
		   + " directory items";

	infoLabel->setText(fmt);
}

void FileTableView::goToFile(const QString &fullFilePath) {
	// qDebug()<<fullFilePath;
	QFileInfo info(fullFilePath);
	// qDebug()<<info.absolutePath();
	cdTo(info.absolutePath());
	directory = info.fileName();
	prevRow = -1;
	setCurrentSelection("");
}

void FileTableView::openContextMenu(QPoint) {

	menu->init();
	menu->popup(QCursor::pos());
}

void FileTableView::commitNewName(QWidget *editor) {
	QString newName = editor->property("text").toString();

	QFileInfo renamedFile = model->fileInfo(currentIndex());
	QFile	  file(renamedFile.absoluteFilePath());
	newName = renamedFile.absolutePath() + "/" + newName;
	// qDebug()<<newName;
	file.rename(newName);
	goToFile(newName);
	// qDebug()<<status;
}

void FileTableView::deleteSelectedFiles() {
	QFileInfoList fileList = getSelectedFiles();

	QString message =
		"Delete " + QString::number(fileList.size()) + " files?\n";

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Deleting ", message,
								  QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::No)
		return;

	auto index = selectionModel()->selectedRows().isEmpty()
					 ? selectionModel()->currentIndex()
					 : selectionModel()->selectedRows().first();

	prevRow = model->mapToSource(index).row();

	bool status;
	int	 counter = 0;
	bool noToAll = false;
	bool yesToAll = false;
	foreach (auto fileInfo, fileList) {
		if (!fileInfo.fileName().compare("..", Qt::CaseInsensitive))
			continue;

		if (!fileInfo.fileName().compare(".", Qt::CaseInsensitive))
			continue;

		if (!fileInfo.isWritable()) {
			QMessageBox::warning(this, "Error!",
								 "You don't have permissions to delete\n"
									 + fileInfo.fileName());
			return;
		}
		if (fileInfo.isDir()) {
			QDir dir(fileInfo.absoluteFilePath());
			if (dir.count() && !yesToAll) {
				if (noToAll)
					continue;
				QMessageBox::StandardButton reply;
				QString						message =
					"Directory " + fileInfo.fileName()
					+ " is not empty!\nAre you sure you want to delete it?\n";
				reply = QMessageBox::question(
					this, "Deleting Directory", message,
					QMessageBox::Yes | QMessageBox::No | QMessageBox::Yes
						| QMessageBox::YesToAll | QMessageBox::NoToAll);
				if (reply == QMessageBox::No)
					break;
				if (reply == QMessageBox::NoToAll) {
					noToAll = true;
					break;
				}
				if (reply == QMessageBox::YesToAll) {
					yesToAll = true;
				}
			}

			status = dir.removeRecursively();
		} else
			status = QFile::remove(fileInfo.absoluteFilePath());

		if (!status) {
			QString msg = "Unable to delede ";
			msg.append(fileInfo.filePath());
			QMessageBox::warning(this, "Error!", msg);
		}
		counter++;
	}
	/*
			if (prevRow >= model->rowCount(rootIndex()) - counter)
					prevRow = model->rowCount(rootIndex()) - counter - 1;
			*/
}

void FileTableView::showHidden(bool show) {
	auto filters = QDir::AllEntries | QDir::NoDot | QDir::System;

	if (show)
		filters |= QDir::Hidden;

	model->setFilter(filters);
}

QString FileTableView::getDirectory() { return model->rootPath(); }

bool FileTableView::isCurrent() const {
	return parent->currentWidget() == this;
}
