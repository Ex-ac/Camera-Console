#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "../SelectedCamera/SelectedCameraWidget.h"
#include "../SystemSettingWidget/SystemControlWidget.h"
#include "../TakePhotoSettingWidget/TakePhotoSettingWidget.h"
#include "../SerialPort/SerialPortWidget.h"
#include "../CameraWidget/CameraWidget.h"
#include "../Communication/Communication.h"

#include <QtWidgets/QSplitter>

#include <QtWidgets/QMainWindow>
#include <QtCore/QList>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <cmath>
#include <QtCore/QDir>
#include <QtCore/QThread>
#include <QtCore/QDebug>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	


public slots:
	void sendSystemCommand(const char *data, uchar count);
	void sendCameraCommand(const char *data);
	void howManyCameraRequest(const char data);
	void setStoreRequest(const char *data);
	void setTimeReauest(const char *data);

	void setWidgetEnable(bool t);


	void treatmentResponse();
	
	void getLastPack();



private:
	void reset(int number);
	void treatmentCameraResponse(QByteArray & byteArray);
	void treatmentSystemResponse(QByteArray & byteArray);

	SerialPortCommuncation _serialPortCommuncation;

	QList<CameraWidget *> _cameraList;

	SelectedCameraWidget *_selectedCameraWidget;
	SystemControlWidget *_systemControlWiget;
	TakePhotoSettingWidget *_takePhotoWidget;
	SerialPortWidget *_serialPortWidget;
	QGridLayout *_rightLayout;

	QThread _thread;
	
	QVBoxLayout *_leftLayout;
	QDir _currentDir;
};



#endif // !MAINWINDOW_H_



