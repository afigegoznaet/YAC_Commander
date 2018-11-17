#include "TableItemDelegate.hpp"
#include <QDebug>
#include <QPainter>

TableItemDelegate::TableItemDelegate(QObject *parent)
	: QItemDelegate(parent), rect(0, 0, 1, 1),
	  grad(QPointF(0, 0), QPointF(1, 1)) {

	grad.setColorAt(0.0, QColor(92, 203, 105, 25));
	grad.setColorAt(0.5, QColor(174, 255, 211, 10));
	grad.setColorAt(1.0, QColor(92, 203, 105, 25));
}

void TableItemDelegate::paint(QPainter *painter,
							  const QStyleOptionViewItem &option,
							  const QModelIndex &index) const {
#if QT_VERSION_MAJOR == 5
#if QT_VERSION_MINOR < 9
	grad.setStart(option.rect.topLeft());
	grad.setFinalStop(option.rect.bottomLeft());
	QRect tempRect;
	tempRect.setLeft(rect.left());
	tempRect.setTop(option.rect.top());
	tempRect.setRight(rect.right());
	tempRect.setBottom(option.rect.bottom());
#else
	const QRect &tempRect = option.rect;
#endif
#else
	const QRect &tempRect = option.rect;
#endif
	if (index.row() == current.row()) {
		// painter->setBackground(grad);
		// painter->setPen(QPen(Qt::blue));
		QItemDelegate::paint(painter, option, index);
		painter->fillRect(tempRect, grad);
	} else {
		QItemDelegate::paint(painter, option, index);
}
}

void TableItemDelegate::currentChanged(QModelIndex current, QModelIndex) {
	this->current = current;
}
