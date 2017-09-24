#include "tableitemdelegate.h"

TableItemDelegate::TableItemDelegate(QObject *parent) :
	QItemDelegate(parent), rect(0,0,1,1){

}

void TableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
							  const QModelIndex &index) const{

	/*qDebug()<<"***********************";
	qDebug()<<index;
	qDebug()<<option.viewItemPosition;
	qDebug()<<painter->pen();
	qDebug()<<painter->brush().color();
	qDebug()<<"***********************";*/
	QItemDelegate::paint(painter, option, index);
	if(index.row()==current.row()){


		QRect tempRect;
		tempRect.setLeft(rect.left());
		tempRect.setTop(option.rect.top());
		tempRect.setRight(rect.right());
		tempRect.setBottom(option.rect.bottom());
		qDebug()<<tempRect.left() << " "<<tempRect.right();
		qDebug()<<rect.left() << "_"<<rect.top()<<"_"<<rect.right()<<"_"<<rect.bottom();
		qDebug()<<option.rect.left() << "_"<<option.rect.top()<<"_"<<option.rect.right()<<"_"<<option.rect.bottom();
		qDebug()<<tempRect.left() << "_"<<tempRect.top()<<"_"<<tempRect.right()<<"_"<<tempRect.bottom();
		QLinearGradient grad(tempRect.topLeft(), tempRect.bottomLeft());
		grad.setColorAt(0.0, QColor(92,203,105,25));
		grad.setColorAt(0.5, QColor(174,255,211,10));
		grad.setColorAt(1.0, QColor(92,203,105,25));
		painter->setBackground(grad);
		painter->setPen(QPen(Qt::blue));
		QItemDelegate::paint(painter, option, index);
		//painter->drawRect(tempRect);
		painter->fillRect(tempRect, grad);
		//QStyleOptionViewItem opt(option);
		//opt.palette.setColor(QPalette::Normal,QPalette::Background, tempBrush););
	}


}


void TableItemDelegate::currentChanged(QModelIndex current,QModelIndex){
	this->current = current;
}
