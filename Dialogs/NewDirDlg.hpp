#ifndef NEWDIR_H
#define NEWDIR_H

#include "NewFileDlg.hpp"

class NewDirDlg : public NewFileDlg {
	Q_OBJECT
public:
	explicit NewDirDlg(QString &label, QString &dirName,
					   QWidget *parent = nullptr);

private:
	QCompleter *fileCompleter;
};
#endif // NEWDIR_H
