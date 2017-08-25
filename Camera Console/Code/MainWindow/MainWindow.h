#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "Code\SelectedCamera\SelectedCameraWidget.h"
#include "Code\SystemSettingWidget\SystemControlWidget.h"
#include "Code\TakePhotoSettingWidget\TakePhotoSettingWidget.h"
#include "Code\SerialPort\SerialPortWidget.h"

#include "QSplitter"

#include <QMainWindow>
#include <QList>
#include <QLabel>
#include <QLayout>
#include <cmath>


#include <qdebug.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	


public slots:
	bool sendSystemCommand(const char *data, uchar count);
	bool sendCameraCommand(const char *data);
	void howManyCameraRequest(const char data);
	void setStoreRequest(const char *data);
	void setTimeReauest(const char *data);

	void setWidgetEnable(bool t);


	void treatmentResponse();




private:
	void reset(int number);


	QList<QPushButton *> _cameraList;

	SelectedCameraWidget *_selectedCameraWidget;
	SystemControlWidget *_systemControlWiget;
	TakePhotoSettingWidget *_takePhotoWidget;
	SerialPortWidget *_serialPortWidget;
	QGridLayout *_rightLayout;
	QSerialPort _serialPort;
	QVBoxLayout *_leftLayout;
};



#endif // !MAINWINDOW_H_



