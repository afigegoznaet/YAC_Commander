#include "TableItemDelegate.hpp"
#include <QDebug>
#include <QPainter>

TableItemDelegate::TableItemDelegate(QObject *parent)
	: QItemDelegate(parent), rect(0, 0, 1, 1),
	  gradFocused(QPointF(0, 0), QPointF(1, 1)),
	  grad(QPointF(0, 0), QPointF(1, 1)), has_focus(false) {

	gradFocused.setColorAt(0.0, QColor(92, 203, 105, 55));
	gradFocused.setColorAt(0.5, QColor(174, 255, 211, 40));
	gradFocused.setColorAt(1.0, QColor(92, 203, 105, 55));

	grad.setColorAt(0.0, QColor(92, 105, 203, 20));
	grad.setColorAt(0.5, QColor(174, 211, 255, 5));
	grad.setColorAt(1.0, QColor(92, 105, 203, 20));
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
#endif
	const QRect &tempRect = option.rect;
#endif
	if (index.row() == current.row()) {
		// painter->setBackground(grad);
		// painter->setPen(QPen(Qt::blue));
		QItemDelegate::paint(painter, option, index);
		if (has_focus)
			painter->fillRect(tempRect, gradFocused);
		else
			painter->fillRect(tempRect, grad);
	} else {
		QItemDelegate::paint(painter, option, index);
	}
}

void TableItemDelegate::currentChanged(QModelIndex current, QModelIndex) {
	this->current = current;
}
