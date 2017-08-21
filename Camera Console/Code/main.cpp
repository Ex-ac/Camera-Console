#include <QtWidgets/QApplication>

//test
#include "SerialPort\SerialPortWidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSerialPort serialPort;
	SerialPortWidget widget(&serialPort);
	widget.show();
	return a.exec();
}
