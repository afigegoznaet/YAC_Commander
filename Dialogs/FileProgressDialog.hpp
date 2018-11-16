#ifndef DIALOG_H
#define DIALOG_H

#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>

namespace Ui {
	class ProgressDialog;
}

class ProgressDialog : public QDialog {
	Q_OBJECT

	// friend class MainWindow;
public:
	explicit ProgressDialog(QWidget *parent = 0,
							Qt::WindowFlags f = Qt::WindowFlags() | Qt::Window);
	~ProgressDialog();

signals:
	void sendErrMsg(QString errorText);
	void hideDialogSignal();
	void dirMoved(int);
	void setStatus(int status);

public slots:
	void onWrite(uint);
	void movementResult(int);
	void dirMovementResult(int);
	void errorMsg(const QString& errorText);
	void hideDialogSlot() { this->hide(); }
	void processFileAction(QFileInfoList fileList, const QString& destination,
						   Qt::DropAction action);
private slots:
	void on_pauseButton_clicked();
	void on_removeButton_clicked();
	void on_abortButton_clicked();

private:
	Ui::ProgressDialog *progress;
	QFuture<void> stub;
	bool status;
	QWaitCondition cond;
	QWaitCondition condStatus;
	QString pauseButtonLabels[2] = {"Continue", "Pause"};
	QMutex moverBlocker;

	void switchText();
	void processItemsInList(void);
	void dirParsing(QDir &dir, QString &action, QString &destination,
					QList<QString> &createdDirs);
	QMessageBox::StandardButton showError(int result);
};

#endif // DIALOG_H
