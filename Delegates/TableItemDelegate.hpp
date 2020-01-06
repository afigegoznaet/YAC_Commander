#ifndef TABLEITEMDELEGATE_H
#define TABLEITEMDELEGATE_H

#include <QItemDelegate>

class TableItemDelegate : public QItemDelegate {
	Q_OBJECT
public:
	TableItemDelegate(QObject *parent = Q_NULLPTR);
	void setRect(QRect rect) { this->rect = rect; }

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option,
			   const QModelIndex &index) const override;
	void drawFocus(QPainter * /*painter*/,
				   const QStyleOptionViewItem & /*option*/,
				   const QRect & /*rect*/) const override {}
public slots:
	void currentChanged(QModelIndex current, QModelIndex);
	void focused(bool focus) { has_focus = focus; }

private:
	QModelIndex				current;
	QRect					rect;
	mutable QLinearGradient gradFocused;
	mutable QLinearGradient grad;
	bool					has_focus{};
};

#endif // TABLEITEMDELEGATE_H
