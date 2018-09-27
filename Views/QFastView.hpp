#ifndef QFASTVIEWER_HPP
#define QFASTVIEWER_HPP

#include <QAbstractScrollArea>
#include <QFontMetrics>

class DataStorage;
class QFastView: public QAbstractScrollArea {
public:
	QFastView(QWidget *parent = nullptr);
	~QFastView();

public slots:
	void setData(DataStorage *pData);
	void clear();
	void showFromOffset(unsigned long long offset);

protected:
	void paintEvent(QPaintEvent *event);
	void paintHex(QPaintEvent *event);
	void paintNormal(QPaintEvent *event);
	int getNextLineStart();
	int getPrevLineStart();
	int getNextPageStart();
	int getPrevPageStart();
	//void keyPressEvent(QKeyEvent *event);
	//void mouseMoveEvent(QMouseEvent *event);
	//void mousePressEvent(QMouseEvent *event);
private:
	DataStorage          *m_pdata;
	uint firstLineIdx = 0;
	int charWidth;
	QByteArray data;
};

#endif // QFASTVIEWER_HPP
