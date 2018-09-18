#include "TableItemDelegate.hpp"

TableItemDelegate::TableItemDelegate(QObject *parent) :
	QItemDelegate(parent), rect(0,0,1,1), grad(QPointF(0,0), QPointF(1,1)){

	grad.setColorAt(0.0, QColor(92,203,105,25));
	grad.setColorAt(0.5, QColor(174,255,211,10));
	grad.setColorAt(1.0, QColor(92,203,105,25));
}

void TableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
							  const QModelIndex &index) const{

	if(index.row()==current.row()){
		//painter->setBackground(grad);
		//painter->setPen(QPen(Qt::blue));
		QItemDelegate::paint(painter, option, index);
		painter->fillRect(option.rect, grad);
	}else
		QItemDelegate::paint(painter, option, index);

}


void TableItemDelegate::currentChanged(QModelIndex current, QModelIndex){
	this->current = current;
}
