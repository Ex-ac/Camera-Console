#include <QtWidgets/QApplication>

#include "./MainWindow/MainWindow.h"

//test
//#include "SerialPort/SerialPortWidget.h"
//#include "SelectedCamera/SelectedCameraWidget.h"
//#include "Code/TakePhotoSettingWidget/TakePhotoSettingWidget.h"
//#include "CameraWidget.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow mainWindow;
	mainWindow.show();
	return a.exec();
}
  