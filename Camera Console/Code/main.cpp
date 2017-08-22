#include <QtWidgets/QApplication>

//test
#include "SerialPort\SerialPortWidget.h"
#include "SelectedCamera\SelectedCameraWidget.h"
#include "Code\TakePhotoSettingWidget\TakePhotoSettingWidget.h"
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSerialPort serialPort;
	SerialPortWidget widget(&serialPort);
	widget.show();
	SelectedCameraWidget widget1(16);
	widget1.show();


	TakePhotoSettingWidget widget2;
	widget2.show();
	return a.exec();
}
