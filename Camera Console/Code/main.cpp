#include <QtWidgets/QApplication>

//test
#include "SerialPort\SerialPortWidget.h"
#include "SelectedCamera\SelectedCameraWidget.h"
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSerialPort serialPort;
	SerialPortWidget widget(&serialPort);
	widget.show();
	SelectedCameraWidget widget1(16);
	widget1.show();
	return a.exec();
}
