#include "MainWindow.h"


#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	
	_selectedCameraWidget = new SelectedCameraWidget(0);
	//_selectedCameraWidget->setFixedSize(_selectedCameraWidget->sizeHint());
	

	_systemControlWiget = new SystemControlWidget;
	//_systemControlWiget->setFixedSize(_systemControlWiget->sizeHint());
	
	connect(_systemControlWiget, SIGNAL(howManyCameraRequest(const char)), this,
		SLOT(howManyCameraRequest(const char)));
	connect(_systemControlWiget, SIGNAL(setStoreRequest(const char *)), this, 
		SLOT(setStoreRequest(const char *)));
	connect(_systemControlWiget, SIGNAL(setTimeReauest(const char *)), this, 
		SLOT(setTimeReauest(const char *)));


	_takePhotoWidget = new TakePhotoSettingWidget;
	//_takePhotoWidget->setFixedSize(_takePhotoWidget->sizeHint());
	connect(_takePhotoWidget, SIGNAL(request(const char *)), this, SLOT(sendCameraCommand(const char *)));
	

	_serialPortWidget = new SerialPortWidget(&_serialPort);
	//_serialPortWidget->setFixedSize(_serialPortWidget->sizeHint());
	connect(_serialPortWidget, SIGNAL(hasOpen(bool)), this, SLOT(setWidgetEnable(bool)));

	_serialPort.setReadBufferSize(1024 * 100); //1M
	connect(&_serialPort, SIGNAL(readyRead()), this, SLOT(treatmentResponse()));

	setWidgetEnable(false);


	_leftLayout = new QVBoxLayout;
	_leftLayout->addWidget(_serialPortWidget);
	_leftLayout->addWidget(_systemControlWiget);
	_leftLayout->addWidget(_selectedCameraWidget);
	_leftLayout->addWidget(_takePhotoWidget);
	_leftLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);

	_rightLayout = new QGridLayout;




	QHBoxLayout *mainlayout = new QHBoxLayout;
	mainlayout->addLayout(_leftLayout);

	mainlayout->addLayout(_rightLayout);
	QWidget *coreWidget = new QWidget;
	coreWidget->setLayout(mainlayout);
	setCentralWidget(coreWidget);

	reset(16);
}



bool MainWindow::sendSystemCommand(const char *data, uchar count)
{
	QByteArray byteData;
	byteData.append(quint8('s'));
	byteData.append(quint8(count));
	byteData.append((char *)data, count);
	byteData.append(quint8('s'));
	qDebug() << byteData.toHex();
	_serialPort.write(byteData);
	if (_serialPort.waitForBytesWritten())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MainWindow::sendCameraCommand(const char *data)
{
	QByteArray byteData;
	byteData.append(quint8('c'));
	byteData.append(quint8(RequestCommandBuffSize + sizeof(quint32)));
	quint32 temp = _selectedCameraWidget->flagOfTheSelectedCamera();
	for (int i = 0; i < 4; i++)
	{
		byteData.append(temp >> (8 * (3 - i)));
	}
	byteData.append((char *)data, RequestCommandBuffSize);
	byteData.append(quint8('c'));
	qDebug() << byteData.toHex();
	_serialPort.write(byteData);
	if (_serialPort.waitForBytesWritten())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MainWindow::howManyCameraRequest(const char data)
{
	sendSystemCommand(&data, 1);
}

void MainWindow::setStoreRequest(const char data[2])
{
	QByteArray byteData;
	byteData.append((char *)data, 2);
	quint32 temp = _selectedCameraWidget->flagOfTheSelectedCamera();
	for (int i = 0; i < 4; i++)
	{
		byteData.append(temp >> (8 * (3 - i)));
	}
	sendSystemCommand((const char *)byteData, byteData.count());
}

void MainWindow::setTimeReauest(const char data[5])
{
	sendSystemCommand(data, 5);
}

void MainWindow::setWidgetEnable(bool t)
{
	_selectedCameraWidget->setEnabled(t);
	_systemControlWiget->setEnabled(t);
	_takePhotoWidget->setEnabled(t);
}

void MainWindow::treatmentResponse()
{
	disconnect(&_serialPort, SIGNAL(readyRead()), this, SLOT(treatmentResponse()));
	QByteArray data;
	data = _serialPort.read(1);
	switch (data[0])
	{
	case 's':
		data = _serialPort.read(2);
		switch (data[0])
		{
		case 0x01:
			reset(data[1]);
			//×´Ì¬À¸
			break;

		case 0x02:
			if (data[1] == char(0xff))
			{
				qDebug() << "success!";
			}
			else
			{

			}
			break;

		case 0x03:
			if (data[1] == char(0xff))
			{
				qDebug() << "success!";
			}
			else
			{

			}
			break;
		default:
			//´íÎó
			;

		}
		break;

	case 'c':
		data = _serialPort.readAll();
		qDebug() << hex << data;

		break;

	default:
		data = _serialPort.readAll();
		qDebug() << QString(data);
		break;
	}
	connect(&_serialPort, SIGNAL(readyRead()), this, SLOT(treatmentResponse()));
}

void MainWindow::reset(int number)
{
	int MaxR = std::sqrt(number);
	if (MaxR * MaxR != number)
	{
		MaxR++;
	}
	_selectedCameraWidget->setNumberOfCamera(number);
	qDeleteAll(_cameraList);
	_cameraList.clear();
	int c = 0, r = 0;

	QSize size((_leftLayout->sizeHint()).height() / MaxR, (_leftLayout->sizeHint()).height() / MaxR);
	QPushButton *temp;
	for (int i = 0; i < number; ++i)
	{
		temp = new QPushButton;
		temp->setEnabled(false);
		
		temp->setFixedSize(size);
		_cameraList.append(temp);
		_rightLayout->addWidget(temp, c, r++);
		if (r == MaxR)
		{
			c++;
			r = 0;
		}
	}
}
