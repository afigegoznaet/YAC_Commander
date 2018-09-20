#ifndef QFASTVIEWER_HPP
#define QFASTVIEWER_HPP

#include <QAbstractScrollArea>

class DataStorage;
class QFastView: public QAbstractScrollArea {
public:
	QFastView(QWidget *parent = 0);
	~QFastView();

public slots:
	void setData(DataStorage *pData);
	void clear();
	void showFromOffset(unsigned long long offset);

protected:
	void paintEvent(QPaintEvent *event);
	void paintHex(QPaintEvent *event);
	void paintNormal(QPaintEvent *event);
	//void keyPressEvent(QKeyEvent *event);
	//void mouseMoveEvent(QMouseEvent *event);
	//void mousePressEvent(QMouseEvent *event);
private:
	DataStorage          *m_pdata;
};

#endif // QFASTVIEWER_HPP
