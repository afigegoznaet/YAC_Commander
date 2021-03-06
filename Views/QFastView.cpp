#include "QFastView.hpp"
//#include "../Dialogs/TextViewerDlg.hpp"

//#include "Views/QFastView.hpp"
#include <QScrollBar>
#include <QPainter>
#include <QSize>
//#include <QPaintEvent>
#include <QKeyEvent>
//#include <QClipboard>
//#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
//#include <stdexcept>
#include "../Dialogs/TextViewerDlg.hpp"

QFastView::QFastView(QWidget *parent)
	: QAbstractScrollArea(parent), pdata(nullptr) {
	QFont theFont("DejaVu Sans Mono", 12);
	QFontMetrics metrix(theFont);
	setFont(theFont);
	setFocusPolicy(Qt::StrongFocus);
	charWidth = metrix.averageCharWidth();
	charHeight = metrix.height();
	connect(this, SIGNAL(updateScrollSize(quint32)), this,
			SLOT(setScrollSize(quint32)));

	connect(verticalScrollBar(), SIGNAL(sliderMoved(int)), this,
			SLOT(moveToLine(int)));
	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this,
			SLOT(moveToLine(int)));
}


QFastView::~QFastView() { delete pdata; }

void QFastView::countLines() {
	if (!pdata)
		return;

	if (!linesPerPage)
		linesPerPage = size().height() / charHeight;

	futureHolder = QtConcurrent::run([&] {
		auto data = pdata->getData(0, pdata->size());
		// QStringView view((QChar*)data.data(), m_pdata->size());
		auto first = data.cbegin();

		while (first != data.cend() && 0 < (data.cend() - first)) {
			first = std::find(first, first + charsPerLine, '\n');
			first++;
			linesCount++;
			// qDebug()<<data.cend() - first;
			// qDebug()<< first - data.cbegin();
		}
		qDebug() << linesCount;
		qDebug() << linesPerPage;
		qDebug() << linesCount - linesPerPage;
		emit this->updateScrollSize(
			std::max<int>(linesCount - linesPerPage, 0));
	});
}

void QFastView::resizeEvent(QResizeEvent *event) {
	// charsPerLine = size().width() / charWidth;
	linesPerPage = size().height() / charHeight;
	maxChars = charsPerLine * linesPerPage;
	emit this->updateScrollSize(std::max<int>(linesCount - linesPerPage, 0));
	QAbstractScrollArea::resizeEvent(event);
}

void QFastView::setScrollSize(quint32 size) {
	// if(size > verticalScrollBar()->maximum())
	verticalScrollBar()->setMaximum(size);
}

void QFastView::moveToLine(int lineNum) {
	// qDebug()<<"New line num: "<<lineNum;
	// qDebug()<<"Prev line num: "<<atomicLineNum;
	if (lineNum < atomicLineNum)
		while (atomicLineNum > lineNum) {
			firstLineIdx = getPrevLineStart();
			--atomicLineNum;
		}
	else {
		while (atomicLineNum < lineNum) {
			firstLineIdx = getNextLineStart();
			++atomicLineNum;
		}
	}

	// atomicLineNum.store(lineNum);
	viewport()->update();
}

void QFastView::setData(DataStorage *pData) {
	verticalScrollBar()->setValue(0);

	delete pdata;
	pdata = pData;
	verticalScrollBar()->setMaximum(pdata->size() / charsPerLine);
	countLines();
	// verticalScrollBar()->setMaximum()
	// m_cursorPos = 0;
	// resetSelection(0);
}


void QFastView::showFromOffset(unsigned long long offset) {
	if (pdata && offset < pdata->size()) {
		// setCursorPos(offset * 2);

		// int cursorY = m_cursorPos / (2 * BYTES_PER_LINE);

		// verticalScrollBar() -> setValue(cursorY);
	}
}

void QFastView::clear() { verticalScrollBar()->setValue(0); }

int QFastView::getNextLineStart() {
	auto charsPerLine = size().width() / charWidth;
	auto newLinePos = data.toStdString().find('\n');
	if (std::string::npos != newLinePos && newLinePos < charsPerLine)
		return firstLineIdx + newLinePos + 1;

	return firstLineIdx + charsPerLine;
}

int QFastView::getPrevLineStart() {

	if (charsPerLine > firstLineIdx)
		return 0;

	auto tempData =
		pdata->getData(firstLineIdx - charsPerLine - 1, charsPerLine);
	auto newLinePos = tempData.toStdString().find_last_of('\n');
	if (std::string::npos != newLinePos && newLinePos < charsPerLine)
		return newLinePos + 1;

	return firstLineIdx - charsPerLine;
}

int QFastView::getNextPageStart() {
	int linesPassed = 0;
	int pos = 0;
	while (linesPassed < linesPerPage) {
		auto newLinePos = data.toStdString().find('\n', pos);
		if (newLinePos == std::string::npos) {
			pos += (maxChars - charsPerLine * linesPassed);
			break;
		}
		linesPassed += newLinePos / charsPerLine + 1;
		pos += (newLinePos + 1);
	}

	if (firstLineIdx + pos > pdata->size())
		return firstLineIdx;

	return firstLineIdx + pos;
}

int QFastView::getPrevPageStart() {
	if (firstLineIdx - maxChars - 1 <= 0)
		return 0;
	int linesRemaining = linesPerPage;
	int pos = maxChars;
	auto data = pdata->getData(firstLineIdx - maxChars - 1, maxChars);
	while (linesRemaining > 0) {
		auto newLinePos = data.toStdString().find_last_of('\n', pos - 1);
		if (newLinePos == std::string::npos)
			return pos - (maxChars - charsPerLine * linesRemaining);
		linesRemaining += (pos - newLinePos) / charsPerLine + 1;
		pos -= newLinePos;
	}
	return std::max(maxChars - pos, 0);
}

void QFastView::paintEvent(QPaintEvent *event) {
	if (!pdata)
		return;

	QPainter painter(viewport());
	QRect rect(event->rect().left(), event->rect().top(),
			   charsPerLine * charWidth, event->rect().height());


	// QBrush def = painter.brush();
	// QBrush selected = QBrush(QColor(0x6d, 0x9e, 0xff, 0xff));
	data = pdata->getData(firstLineIdx, 8192);
	data.replace('\0', '?');
	QString text(data);
	painter.drawText(rect, text);
}


void QFastView::keyPressEvent(QKeyEvent *event) {
	bool setVisible = false;

	switch (event->key()) {
	case (Qt::Key_Down):
		verticalScrollBar()->setValue(verticalScrollBar()->value() + 1);
		firstLineIdx = getNextLineStart();
		setVisible = true;
		break;
	case (Qt::Key_Up):
		verticalScrollBar()->setValue(verticalScrollBar()->value() - 1);
		firstLineIdx = getPrevLineStart();
		setVisible = true;
		break;
	case (Qt::Key_PageDown):
		verticalScrollBar()->setValue(verticalScrollBar()->value()
									  - linesPerPage);
		firstLineIdx = getNextPageStart();
		setVisible = true;
		break;
	case (Qt::Key_PageUp):
		verticalScrollBar()->setValue(verticalScrollBar()->value()
									  + linesPerPage);
		firstLineIdx = getPrevPageStart();
		setVisible = true;
		break;
	case (Qt::Key_End):
		if (event->modifiers() & Qt::ControlModifier) {
			verticalScrollBar()->setValue(verticalScrollBar()->maximum());
			setVisible = true;
		}
		break;
	case (Qt::Key_Home):
		if (event->modifiers() & Qt::ControlModifier) {
			verticalScrollBar()->setValue(0);
			setVisible = true;
		}
		break;
	default:
		QAbstractScrollArea::keyPressEvent(event);
	}

	atomicLineNum.store(verticalScrollBar()->value());

	viewport()->update();
}
/*
void QFastView::mouseMoveEvent(QMouseEvent * event)
{
	int actPos = cursorPos(event->pos());
	setCursorPos(actPos);
	setSelection(actPos);

	viewport() -> update();
}

void QFastView::mousePressEvent(QMouseEvent * event)
{
	int cPos = cursorPos(event->pos());

	if((QApplication::keyboardModifiers() & Qt::ShiftModifier) && event ->
button() == Qt::LeftButton) setSelection(cPos); else resetSelection(cPos);

	setCursorPos(cPos);

	viewport() -> update();
}
*/
