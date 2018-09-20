#include "QFastView.hpp"
#include "../Dialogs/TextViewerDlg.hpp"

#include "Views/QFastView.hpp"
#include <QScrollBar>
#include <QPainter>
#include <QSize>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>

#include <QDebug>
#include <stdexcept>
#include "../Dialogs/TextViewerDlg.hpp"

QFastView::QFastView(QWidget *parent):
QAbstractScrollArea(parent),
m_pdata(NULL)
{
	setFont(QFont("DejaVu Sans Mono", 12));

	setFocusPolicy(Qt::StrongFocus);
}


QFastView::~QFastView()
{
	if(m_pdata)
		delete m_pdata;
}

void QFastView::setData(DataStorage *pData)
{
	verticalScrollBar()->setValue(0);
	if(m_pdata)
		delete m_pdata;
	m_pdata = pData;
	//m_cursorPos = 0;
	//resetSelection(0);
}


void QFastView::showFromOffset(unsigned long long offset)
{
	if(m_pdata && offset < m_pdata->size())
	{
		//setCursorPos(offset * 2);

		//int cursorY = m_cursorPos / (2 * BYTES_PER_LINE);

		//verticalScrollBar() -> setValue(cursorY);
	}
}

void QFastView::clear()
{
	verticalScrollBar()->setValue(0);
}

/*
QSize QFastView::fullSize() const
{
	if(!m_pdata)
		return QSize(0, 0);

	unsigned long long width = m_posAscii + (BYTES_PER_LINE * m_charWidth);
	unsigned long long height = m_pdata->size() / BYTES_PER_LINE;
	if(m_pdata->size() % BYTES_PER_LINE)
		height++;

	height *= m_charHeight;

	return QSize(width, height);
}

void QFastView::keyPressEvent(QKeyEvent *event)
{
	bool setVisible = false;

/*****************************************************************************/
/* Cursor movements */
/*****************************************************************************
	if(event->matches(QKeySequence::MoveToNextChar))
	{
		setCursorPos(m_cursorPos + 1);
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToPreviousChar))
	{
		setCursorPos(m_cursorPos - 1);
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToEndOfLine))
	{
		setCursorPos(m_cursorPos | ((BYTES_PER_LINE * 2) - 1));
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToStartOfLine))
	{
		setCursorPos(m_cursorPos | (m_cursorPos % (BYTES_PER_LINE * 2)));
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToPreviousLine))
	{
		setCursorPos(m_cursorPos - BYTES_PER_LINE * 2);
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToNextLine))
	{
		setCursorPos(m_cursorPos + BYTES_PER_LINE * 2);
		resetSelection(m_cursorPos);
		setVisible = true;
	}

	if(event->matches(QKeySequence::MoveToNextPage))
	{
		setCursorPos(m_cursorPos + (viewport()->height() / m_charHeight - 1) * 2 * BYTES_PER_LINE);
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToPreviousPage))
	{
		setCursorPos(m_cursorPos - (viewport()->height() / m_charHeight - 1) * 2 * BYTES_PER_LINE);
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToEndOfDocument))
	{
		if(m_pdata)
			setCursorPos(m_pdata->size() * 2);
		resetSelection(m_cursorPos);
		setVisible = true;
	}
	if(event->matches(QKeySequence::MoveToStartOfDocument))
	{
		setCursorPos(0);
		resetSelection(m_cursorPos);
		setVisible = true;
	}

/*****************************************************************************/
/* Select commands */
/*****************************************************************************
	if (event->matches(QKeySequence::SelectAll))
	{
		resetSelection(0);
		if(m_pdata)
			setSelection(2 * m_pdata->size() + 1);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectNextChar))
	{
		int pos = m_cursorPos + 1;
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectPreviousChar))
	{
		int pos = m_cursorPos - 1;
		setSelection(pos);
		setCursorPos(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectEndOfLine))
	{
		int pos = m_cursorPos - (m_cursorPos % (2 * BYTES_PER_LINE)) + (2 * BYTES_PER_LINE);
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectStartOfLine))
	{
		int pos = m_cursorPos - (m_cursorPos % (2 * BYTES_PER_LINE));
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectPreviousLine))
	{
		int pos = m_cursorPos - (2 * BYTES_PER_LINE);
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectNextLine))
	{
		int pos = m_cursorPos + (2 * BYTES_PER_LINE);
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}

	if (event->matches(QKeySequence::SelectNextPage))
	{
		int pos = m_cursorPos + (((viewport()->height() / m_charHeight) - 1) * 2 * BYTES_PER_LINE);
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectPreviousPage))
	{
		int pos = m_cursorPos - (((viewport()->height() / m_charHeight) - 1) * 2 * BYTES_PER_LINE);
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectEndOfDocument))
	{
		int pos = 0;
		if(m_pdata)
			pos = m_pdata->size() * 2;
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}
	if (event->matches(QKeySequence::SelectStartOfDocument))
	{
		int pos = 0;
		setCursorPos(pos);
		setSelection(pos);
		setVisible = true;
	}

	if (event->matches(QKeySequence::Copy))
	{
		if(m_pdata)
		{
			QString res;
			int idx = 0;
			int copyOffset = 0;

			QByteArray data = m_pdata->getData(m_selectBegin / 2, (m_selectEnd - m_selectBegin) / 2 + 1);
			if(m_selectBegin % 2)
			{
				res += QString::number((data.at((idx+1) / 2) & 0xF), 16);
				res += " ";
				idx++;
				copyOffset = 1;
			}

			int selectedSize = m_selectEnd - m_selectBegin;
			for (;idx < selectedSize; idx+= 2)
			{
				QString val = QString::number((data.at((copyOffset + idx) / 2) & 0xF0) >> 4, 16);
				if(idx + 1 < selectedSize)
				{
					val += QString::number((data.at((copyOffset + idx) / 2) & 0xF), 16);
					val += " ";
				}
				res += val;

				if((idx/2) % BYTES_PER_LINE == (BYTES_PER_LINE - 1))
					res += "\n";
			}
			QClipboard *clipboard = QApplication::clipboard();
			clipboard -> setText(res);
		}
	}

	if(setVisible)
		ensureVisible();
	viewport() -> update();
}

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

	if((QApplication::keyboardModifiers() & Qt::ShiftModifier) && event -> button() == Qt::LeftButton)
		setSelection(cPos);
	else
		resetSelection(cPos);

	setCursorPos(cPos);

	viewport() -> update();
}

unsigned long long QFastView::cursorPos(const QPoint &position)
{
	int pos = -1;

	if (((unsigned long long)position.x() >= m_posHex)
			&& ((unsigned long long)position.x() < (m_posHex + HEXCHARS_IN_LINE * m_charWidth)))
	{
		int x = (position.x() - m_posHex) / m_charWidth;
		if ((x % 3) == 0)
			x = (x / 3) * 2;
		else
			x = ((x / 3) * 2) + 1;

		int firstLineIdx = verticalScrollBar() -> value();
		int y = (position.y() / m_charHeight) * 2 * BYTES_PER_LINE;
		pos = x + y + firstLineIdx * BYTES_PER_LINE * 2;
	}
	return pos;
}


void QFastView::resetSelection()
{
	m_selectBegin = m_selectInit;
	m_selectEnd = m_selectInit;
}

void QFastView::resetSelection(int pos)
{
	if (pos < 0)
		pos = 0;

	m_selectInit = pos;
	m_selectBegin = pos;
	m_selectEnd = pos;
}

void QFastView::setSelection(unsigned long long pos)
{

	if (pos >= m_selectInit)
	{
		m_selectEnd = pos;
		m_selectBegin = m_selectInit;
	}
	else
	{
		m_selectBegin = pos;
		m_selectEnd = m_selectInit;
	}
}


void QFastView::setCursorPos(int position)
{
	if(position < 0)
		position = 0;

	int maxPos = 0;
	if(m_pdata)
	{
		maxPos = m_pdata->size() * 2;
		if(m_pdata->size() % BYTES_PER_LINE)
			maxPos++;
	}

	if(position > maxPos)
		position = maxPos;

	m_cursorPos = position;
}

void QFastView::ensureVisible()
{
	QSize areaSize = viewport()->size();

	int firstLineIdx = verticalScrollBar() -> value();
	int lastLineIdx = firstLineIdx + areaSize.height() / m_charHeight;

	int cursorY = m_cursorPos / (2 * BYTES_PER_LINE);

	if(cursorY < firstLineIdx)
		verticalScrollBar() -> setValue(cursorY);
	else if(cursorY >= lastLineIdx)
		verticalScrollBar() -> setValue(cursorY - areaSize.height() / m_charHeight + 1);
}
*/

void QFastView::paintEvent(QPaintEvent *event){
	if(!m_pdata)
		return;

	QPainter painter(viewport());
/*
	QSize areaSize = viewport()->size();
	QSize  widgetSize = fullSize();
	verticalScrollBar()->setPageStep(areaSize.height() / m_charHeight);
	verticalScrollBar()->setRange(0, (widgetSize.height() - areaSize.height()) / m_charHeight + 1);

	uint firstLineIdx = verticalScrollBar() -> value();

	uint lastLineIdx = firstLineIdx + areaSize.height() / m_charHeight;
	if(false || lastLineIdx > m_pdata->size() / BYTES_PER_LINE)
	{
		lastLineIdx = m_pdata->size() / BYTES_PER_LINE;
		if(m_pdata->size() % BYTES_PER_LINE)
			lastLineIdx++;
	}
*/
	uint firstLineIdx = verticalScrollBar() -> value();
	painter.fillRect(event->rect(), this->palette().color(QPalette::Base));

	//int yPosStart = m_charHeight;

	QBrush def = painter.brush();
	QBrush selected = QBrush(QColor(0x6d, 0x9e, 0xff, 0xff));
	QByteArray data = m_pdata->getData(firstLineIdx * 32, 8192);
	data.replace('\0', '?');
	QString text(data);
	painter.drawText(event->rect(), text);
}
