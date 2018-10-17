#ifndef QFASTVIEWER_HPP
#define QFASTVIEWER_HPP

#include <QAbstractScrollArea>
#include <QFontMetrics>
#include <QFuture>

class DataStorage;
class QFastView: public QAbstractScrollArea {
	Q_OBJECT
public:
	QFastView(QWidget *parent = nullptr);
	~QFastView();
signals:
	void updateScrollSize(quint32 size);

public slots:
	void setData(DataStorage *pData);
	void clear();
	void showFromOffset(unsigned long long offset);
	void setScrollSize(quint32 size);
private slots:
	void moveToLine(int lineNum);

protected:
	void paintEvent(QPaintEvent *event);
	void paintHex(QPaintEvent *event);
	void paintNormal(QPaintEvent *event);
	int getNextLineStart();
	int getPrevLineStart();
	int getNextPageStart();
	int getPrevPageStart();
	void resizeEvent(QResizeEvent *event) override;
	void keyPressEvent(QKeyEvent *event);
	//void mouseMoveEvent(QMouseEvent *event);
	//void mousePressEvent(QMouseEvent *event);
private:
	void countLines();
	int getNextLinesPage();
	int getPrevLinesPerPage();

	QByteArray data;
	DataStorage *m_pdata;
	QFuture<void> futureHolder;

	uint firstLineIdx = 0;
	int charWidth;
	int charHeight;
	int linesPerPage;
	int maxChars;
	int charsPerLine = 80;
	quint32 linesCount = 0;
	QAtomicInt atomicLineNum = 0;
};

#endif // QFASTVIEWER_HPP
