#ifndef Q_HEX_VIEWER_H_
#define Q_HEX_VIEWER_H_

#include <QAbstractScrollArea>
#include <QByteArray>
#include <QFile>

class QHexView: public QAbstractScrollArea

{
	public:
		class DataStorage
		{
			public:
				virtual ~DataStorage() {};
				virtual QByteArray getData(unsigned long long position, unsigned long long length) = 0;
				virtual unsigned long long size() = 0;
		};


		class DataStorageArray: public DataStorage
		{
			public:
				DataStorageArray(const QByteArray &arr);
				virtual QByteArray getData(unsigned long long position, unsigned long long length);
				virtual unsigned long long  size();
			private:
				QByteArray    m_data;
		};

		class DataStorageFile: public DataStorage
		{
			public:
				DataStorageFile(const QString &fileName);
				virtual QByteArray getData(std::size_t position, std::size_t length);
				virtual unsigned long long  size();
			private:
				QFile      m_file;
		};



		QHexView(QWidget *parent = 0);
		~QHexView();

	public slots:
		void setData(DataStorage *pData);
		void clear();
		void showFromOffset(unsigned long long offset);

	protected:
		void paintEvent(QPaintEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
	private:
		DataStorage          *m_pdata;
		unsigned long long           m_posAddr;
		unsigned long long           m_posHex;
		unsigned long long           m_posAscii;
		unsigned long long           m_charWidth;
		unsigned long long           m_charHeight;


		unsigned long long           m_selectBegin;
		unsigned long long           m_selectEnd;
		unsigned long long           m_selectInit;
		unsigned long long           m_cursorPos;


		QSize fullSize() const;
		void resetSelection();
		void resetSelection(int pos);
		void setSelection(int pos);
		void ensureVisible();
		void setCursorPos(int pos);
		unsigned long long cursorPos(const QPoint &position);
};

#endif
