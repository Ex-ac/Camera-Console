#include "./MainWindow.h"

#include "../Protocols/generalizeddatastructure.h"
#include <QtWidgets/QPushButton>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{

	_currentDir = QDir::current();
	qDebug() << _currentDir.path();

	_selectedCameraWidget = new SelectedCameraWidget(0);
	//connect(_selectedCameraWidget, SIGNAL(getLastPackClicked()), this, )
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

	_serialPortWidget = new SerialPortWidget(&_serialPortCommuncation.serialPort());
	//_serialPortWidget->setFixedSize(_serialPortWidget->sizeHint());
	connect(_serialPortWidget, SIGNAL(hasOpen(bool)), this, SLOT(setWidgetEnable(bool)));
	
	connect(&_serialPortCommuncation, SIGNAL(newDataIn(int)), this, SLOT(treatmentResponse()));
	
	
	//_serialPortCommuncation.moveToThread(&_thread);

	//_thread.start();
	//	_serialPort.setReadBufferSize(1024 * 1024 * 100); //1M
	//	connect(&_serialPort, SIGNAL(readyRead()), this, SLOT(treatmentResponse()));

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

void MainWindow::sendSystemCommand(const char *data, uchar count)
{
	QByteArray byteData;
	byteData.append(quint8('S'));
	byteData.append((char *)data, count);
	byteData.append(11 - count, 0xff);

	qDebug() << byteData.toHex() << "\tSize: " << byteData.size();

	_serialPortCommuncation.sendData(byteData);
}

void MainWindow::sendCameraCommand(const char *data)
{
	QByteArray byteData;
	byteData.append(quint8('C'));
	//byteData.append(quint8(RequestCommandBuffSize + sizeof(quint32)));
	quint32 temp = _selectedCameraWidget->flagOfTheSelectedCamera();
	for (int i = 0; i < 4; i++)
	{
		byteData.append(temp >> (8 * (3 - i)));
	}
	byteData.append((char *)data, RequestCommandBuffSize);
	//byteData.append(quint8('c'));
	

	//for (int i = 0; i < 1024; ++i)
	//{
	//	byteData.append(byteData);
	//}
	qDebug() << byteData.toHex() << "\tSize: " << byteData.size();

	_serialPortCommuncation.sendData(byteData);
	if (uchar(*(data + 4)) == uchar(MasterProtocols::RequestCommand::TakePicture))
	{

		QString tempDir = tr(u8"/%1").arg(QDateTime::currentDateTime().toTime_t());
		_currentDir.mkdir(tempDir);
		QDir::setCurrent(_currentDir.path() + "/" + tempDir);
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
	_serialPortCommuncation.intervalReadStart(t);
	_selectedCameraWidget->setEnabled(t);
	_systemControlWiget->setEnabled(t);
	_takePhotoWidget->setEnabled(t);
}

void MainWindow::treatmentResponse()
{

	QByteArray temp;


	
	while ((temp = _serialPortCommuncation.popBuff()) != QByteArray())
	{
		if (temp.size() == 4)
		{
			treatmentSystemResponse(temp);
		}
		else
		{
			treatmentCameraResponse(temp);
		}

	}
	
}

void MainWindow::getLastPack()
{
	for (auto i : _cameraList)
	{
		if (i->isAllPackHasGet())
		{
			for (auto j : i->packNeedGet())
			{
				;
			}
		}
	}
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

	QSize size((_leftLayout->sizeHint()).height() / MaxR * 3 / 2, (_leftLayout->sizeHint()).height() / MaxR);
	CameraWidget *temp;
	for (int i = 0; i < number; ++i)
	{
		temp = new CameraWidget(i);

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

void MainWindow::treatmentCameraResponse(QByteArray &byteArray)
{
	int id = byteArray[3];

	unsigned short sizeOfByte = (uchar(byteArray[1]) << 8) + uchar(byteArray[2]);

	PackBuff *packBuff;
	unsigned short numberOfPack;
	switch (uchar(byteArray[4]))
	{
	case uchar(MasterProtocols::AnswerCommand::Focus):
	{
		unsigned short zoom = 256 * uchar(byteArray[6]) + uchar(byteArray[7]);
		_cameraList[id]->setZoom(zoom);
	}
	break;

	case uchar(MasterProtocols::AnswerCommand::TakePicture):
	{
		unsigned int sizeOfByte = (uchar(byteArray[6]) << 16) + (uchar(byteArray[7]) << 8) + uchar(byteArray[8]);
		unsigned short sizeOfPack = (uchar(byteArray[9]) << 8) + uchar(byteArray[10]);
		PicturePackInfo info{sizeOfByte, sizeOfPack};
		_cameraList[id]->setPicturePackInfo(info);
	}
	break;

	case uchar(MasterProtocols::AnswerCommand::GetPicturePack):
	{
		//unsigned short numberOfPack = (uchar(byteArray[6]) << 8) + uchar(byteArray[7]);
		//PackBuff packBuff(sizeOfByte - AnswerCommandBuffSize, numberOfPack);
//		qCopy(byteArray.begin() + 11, byteArray.end() - 2, packBuff.getPointToPackBuff());
		_cameraList[id]->dealDataPack(byteArray);
	}
	break;
	default:
		qDebug() << byteArray.toHex();
		qDebug() << byteArray.size();
		qDebug() << "Unkown error";
		break;
	}

	//qDebug() << hex << byteArray;
}

void MainWindow::treatmentSystemResponse(QByteArray &byteArray)
{
	switch (byteArray[1])
	{
	case 0x01:
		reset(byteArray[2]);
		//状态栏
		break;

	case 0x02:
		if (byteArray[2] == char(0xff))
		{
			qDebug() << "success!";
		}
		else
		{
		}
		break;

	case 0x03:
		if (byteArray[2] == char(0xff))
		{
			qDebug() << "success!";
		}
		else
		{
		}
		break;
	default:
		//错误
		;
	}
}
