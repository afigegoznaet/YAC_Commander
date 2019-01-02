#include "ItemContextMenu.hpp"
#include "Views/FileTableView.hpp"
#include <QDebug>
#include <QGuiApplication>
#include <QClipboard>
#include <QMimeData>
#include <QMimeDatabase>
#include <Models/OrderedFileSystemModel.hpp>

#ifdef __linux__

#include <KConfigGroup>
#include <KSharedConfig>
#define Q_DECL_CONSTRUCTOR_DEPRECATED
#include <KFileItemActions>
#include <KFileItemListProperties>
#include <KPropertiesDialog>
#include <KActionCollection>
#endif


ItemContextMenu::ItemContextMenu(QWidget *parent) : QMenu(parent) {

	initCommon();
	this->parent = qobject_cast<FileTableView *>(parent);
	// clipboard = QGuiApplication::clipboard();
	connect(this, &QMenu::aboutToHide, [&]() {
#ifdef __linux__
	// qDebug()<<fileItemActions->children();
	// fileItemActions->deleteLater();
#endif
		for (const auto &action : this->actions())
			if (commonActions.contains(action))
				continue;
			else
				removeAction(action);

		;
	});

	cutActionIndicator.reserve(4);
	cutActionIndicator[0] = 2;
	cutActionIndicator[1] = '\0';
	cutActionIndicator[2] = '\0';
	cutActionIndicator[3] = '\0';
	cutActionPadding.reserve(1044);
	cutActionPadding[0] = 255;
	cutActionPadding[1] = 255;
	cutActionPadding[2] = 255;
	cutActionPadding[3] = 255;
}

void ItemContextMenu::init() {
	selectedFiles = parent->getSelectedFiles();
	if (selectedFiles.length() == 1
		&& (!selectedFiles.first().fileName().compare("..")
			|| selectedFiles.first().fileName().isEmpty())) {
		cutAction->setDisabled(true);
		copyAction->setDisabled(true);
		renameAction->setDisabled(true);
		deleteAction->setDisabled(true);
	}

	auto data = QGuiApplication::clipboard()->mimeData();
	if (!data->hasUrls())
		pasteAction->setDisabled(true);
	else
		pasteAction->setEnabled(true);

	initFolder();
	initFile();
}

void ItemContextMenu::initCommon() {

	cutAction = addAction("Cut", this, &ItemContextMenu::cutToClipboard,
						  QKeySequence(tr("Ctrl+X")));
	copyAction = addAction("Copy", this, &ItemContextMenu::copyToClipboard,
						   QKeySequence(tr("Ctrl+C")));
	pasteAction = addAction("Paste", this, &ItemContextMenu::pasteFromClipboard,
							QKeySequence(tr("Ctrl+V")));

	addSeparator();
	deleteAction =
		addAction("Delete selected", this, &ItemContextMenu::deleteItems,
				  QKeySequence(tr("Del")));
	renameAction = addAction("Rename", this, &ItemContextMenu::rename,
							 QKeySequence(tr("Ins")));
	addSeparator();
	commonActions << pasteAction << copyAction << cutAction << deleteAction
				  << renameAction;
	addSeparator();
}
void ItemContextMenu::initFile() {

#ifdef __linux__

	KFileItemActions *fileItemActions = new KFileItemActions(this);

	fileItemActions->setParentWidget(this);

	// qDebug()<<"Adding files to list";
	KFileItemList kList;
	for (const auto &fileInfo : selectedFiles) {
		struct stat buf {};
		stat(fileInfo.absoluteFilePath().toLocal8Bit().data(), &buf);
		KFileItem kItem(QUrl::fromLocalFile(fileInfo.absoluteFilePath()),
						QMimeDatabase().mimeTypeForFile(fileInfo).name(),
						buf.st_mode);
		kList.append(kItem);
	}

	KFileItemListProperties kprops(kList);

	fileItemActions->setItemListProperties(kprops);

	fileItemActions->addOpenWithActionsTo(
		this, QStringLiteral("DesktopEntryName != '%1'")
				  .arg(qApp->desktopFileName()));
	fileItemActions->addServiceActionsTo(this);
	fileItemActions->addPluginActionsTo(this);

	initProperties(std::move(kList));
#endif
}
void ItemContextMenu::initFolder() {}

void ItemContextMenu::cutToClipboard() {
	auto data = parent->getModel()->mimeData(parent->getSelectedIndexes());
#ifdef WIN32
	data->setData(
		"application/x-qt-windows-mime;value=\"Preferred DropEffect\"",
		cutActionIndicator);
	data->setData("application/x-qt-windows-mime;value=\"DropDescription\"",
				  cutActionPadding);
#else
	data->setData("application/x-kde-cutselection", "1");
#endif
	// qDebug()<<data->formats();
	QGuiApplication::clipboard()->setMimeData(data);
}

void ItemContextMenu::copyToClipboard() {
	auto data = parent->getModel()->mimeData(parent->getSelectedIndexes());
	QGuiApplication::clipboard()->setMimeData(data);
}

void ItemContextMenu::pasteFromClipboard() {
	const auto &clipboard = QGuiApplication::clipboard();
	auto data = clipboard->mimeData();

	bool move = false;
#ifdef WIN32
	move =
		(data->data(
				 "application/x-qt-windows-mime;value=\"Preferred DropEffect\"")
			 .at(0)
		 == 2);
#else
	move = data->data("application/x-kde-cutselection").length() > 0;
#endif
	if (move) {
		parent->getModel()->dropMimeData(data, Qt::MoveAction, 1, 0,
										 QModelIndex());
		selIndexes.clear();
		pasteAction->setDisabled(true);
	} else
		parent->getModel()->dropMimeData(data, Qt::CopyAction, 1, 0,
										 QModelIndex());
}

void ItemContextMenu::deleteItems() { parent->deleteSelectedFiles(); }

void ItemContextMenu::rename() { parent->openEditor(selIndexes.first()); }

#ifdef __linux__
void ItemContextMenu::initProperties(KFileItemList &&kList) {

	auto propDlg = [&parent = this->parent, kList = std::move(kList)] {
		KPropertiesDialog *dialog = nullptr;
		if (kList.isEmpty()) {
			return;
		} else {
			dialog = new KPropertiesDialog(std::move(kList), parent);
		}

		dialog->setAttribute(Qt::WA_DeleteOnClose);
		dialog->show();
		dialog->raise();
		dialog->activateWindow();
	};

	addSeparator();
	addAction("Properties", propDlg, QKeySequence(tr("Ctrl+P")));
}
#endif
