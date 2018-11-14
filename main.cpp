#include "MainWindow.hpp"
#include <QSettings>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("Roman's Dark Star Laptop");
	QCoreApplication::setApplicationName("YAC Commander");
	QSettings settings;
	MainWindow w;
	w.setWindowTitle("YAC Commander");
	w.show();
	w.parseParams(argc, argv);

	qApp->setDesktopFileName("YAC Commander");
	return a.exec();
}
