#include "TableItemDelegate.hpp"
#include <QDebug>
#include <QPainter>

TableItemDelegate::TableItemDelegate(QObject *parent)
	: QItemDelegate(parent), rect(0, 0, 1, 1),
	  gradFocused(QPointF(0, 0), QPointF(1, 1)),
	  grad(QPointF(0, 0), QPointF(1, 1)), has_focus(false) {


	QColor dark =
		qobject_cast<QWidget *>(parent)->style()->standardPalette().color(
			QPalette::Highlight);

	QColor light(dark);
	dark.setAlpha(25);
	light.setAlpha(10);
	grad.setColorAt(0.0, dark);
	grad.setColorAt(0.5, light);
	grad.setColorAt(1.0, dark);

	dark.setAlpha(60);
	light.setAlpha(20);
	gradFocused.setColorAt(0.0, dark);
	gradFocused.setColorAt(0.5, light);
	gradFocused.setColorAt(1.0, dark);

	/*
	grad.setColorAt(0.0, QColor(92, 105, 203, 20));
	grad.setColorAt(0.5, QColor(174, 211, 255, 5));
	grad.setColorAt(1.0, QColor(92, 105, 203, 20));
	*/
}

void TableItemDelegate::paint(QPainter *painter,
							  const QStyleOptionViewItem &option,
							  const QModelIndex &index) const {
#if QT_VERSION_MAJOR == 5
#if QT_VERSION_MINOR < 9
	grad.setStart(option.rect.topLeft());
	grad.setFinalStop(option.rect.bottomLeft());
	gradFocused.setStart(option.rect.topLeft());
	gradFocused.setFinalStop(option.rect.bottomLeft());
	QRect tempRect;
	tempRect.setLeft(rect.left());
	tempRect.setTop(option.rect.top());
	tempRect.setRight(rect.right());
	tempRect.setBottom(option.rect.bottom());
#else
	const QRect &tempRect = option.rect;
#endif
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
