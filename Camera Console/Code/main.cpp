#include <QtWidgets/QApplication>

#include "Code\MainWindow\MainWindow.h"

//test
//#include "SerialPort\SerialPortWidget.h"
//#include "SelectedCamera\SelectedCameraWidget.h"
//#include "Code\TakePhotoSettingWidget\TakePhotoSettingWidget.h"
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow mainWindow;
	mainWindow.show();
	return a.exec();
}
