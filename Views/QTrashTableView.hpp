#ifndef QTRASHTABLEVIEW_HPP
#define QTRASHTABLEVIEW_HPP

#include "FileTableView.hpp"
#include <KFileItem>
#include "Models/QTrashTableModel.hpp"

class QLabel;
class TableItemDelegate;
class FileTabSelector;
class QTrashTableModel;

class QTrashTableView : public FileTableView {
	Q_OBJECT
public:
	explicit QTrashTableView(const QDir &directory, QWidget *parent = nullptr);

	KFileItemList getSelectedFiles();

	void init() override;
	QString getDirectory() const override { return directory; }

signals:
	void trashNotEmpty();
	void trashEmpty();

public slots:

	void on_doubleClicked(const QModelIndex &index) override;
	// void headerClicked(int section);
	void updateInfo();
	void openContextMenu(QPoint) override;
	void restoreItems();
	void purgeItems();


protected:
	// void keyPressEvent(QKeyEvent *event) override;
	bool parseItems(size_t &totalSize, int &dirCount, int &fileCount,
					const KFileItemList &items);
	QList<QUrl> getSelectedItems(QList<int> &rows);

private:
	QTabWidget *metaTab = nullptr;
	QMenu *menu = nullptr;

	QTrashTableModel *getTrashModel() {
		return qobject_cast<QTrashTableModel *>(model);
	}

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // QTRASHTABLEVIEW_HPP
