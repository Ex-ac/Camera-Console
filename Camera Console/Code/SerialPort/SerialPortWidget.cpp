#include "SerialPortWidget.h"

#include <qpushbutton.h>
#include <qcombobox.h>
#include <qserialportinfo.h>
#include <qlayout.h>
#include <qlist.h>
#include <QLabel>
#include <QSpinBox>

SerialPortWidget::SerialPortWidget(QSerialPort *serialport, QWidget *parent)
	:QWidget(parent), _serialPort(serialport)
{
	bool isOpen = _serialPort->isOpen();

	_openCloseButton = new QPushButton;
	connect(_openCloseButton, SIGNAL(clicked()), this, SLOT(openClosePort()));

	_flushButton = new QPushButton(tr("刷新"));
	connect(_flushButton, SIGNAL(clicked()), this, SLOT(flushAvaliablePort()));

	_portNameLabel = new QLabel(tr("端口名称:"));
	_portNameComboBox = new QComboBox;
	_portNameLabel->setBuddy(_portNameComboBox);
	

	_baudRateLabel = new QLabel(tr("波特率："));
	_baudRateComboBox = new QComboBox;
	_baudRateComboBox->addItem(tr("4800"), qint32(4800));
	_baudRateComboBox->addItem(tr("9600"), qint32(9600));
	_baudRateComboBox->addItem(tr("19200"), qint32(19200));
	_baudRateComboBox->addItem(tr("38400"), qint32(38400));
	_baudRateComboBox->addItem(tr("57600"), qint32(57600));
	_baudRateComboBox->addItem(tr("115200"), qint32(11520));
	_baudRateComboBox->addItem(tr("Other"), qint32(-1));
	_baudRateComboBox->addItem(tr("未知"));
	_baudRateLabel->setBuddy(_baudRateComboBox);

	connect(_baudRateComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on__baudRateComboBox_currentIndexChanged(int)));

	_baudRateSpinBox = new QSpinBox;
	_baudRateSpinBox->setEnabled(false);
	_baudRateSpinBox->setMinimum(0);
	connect(_baudRateSpinBox, SIGNAL(valueChanged(int i)), this,
		SLOT(on__baudRateSpinBox_valueChanged()));

	_dataBitsLabel = new QLabel(tr("数据位："));
	_dataBitsComboBox = new QComboBox;
	_dataBitsComboBox->addItem(tr("5"), 5);
	_dataBitsComboBox->addItem(tr("6"), 6);
	_dataBitsComboBox->addItem(tr("7"), 7);
	_dataBitsComboBox->addItem(tr("8"), 8);
	_dataBitsComboBox->addItem(tr("未知"), -1);
	

	_dataBitsLabel->setBuddy(_dataBitsComboBox);
	connect(_dataBitsComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on__dataBitsComboBox_currentIndexChanged(int)));

	_stopBitsLabel = new QLabel(tr("结束位："));
	_stopBitsComboBox = new QComboBox;
	_stopBitsComboBox->addItem(tr("1"));
	_stopBitsComboBox->addItem(tr("1.5"));
	_stopBitsComboBox->addItem(tr("2"));
	
	_stopBitsLabel->setBuddy(_stopBitsComboBox);
	connect(_stopBitsComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on__stopBitsComboBox_currentIndexChanged(int)));

	_parityLabel = new QLabel(tr("校验位："));
	_parityComboBox = new QComboBox;
	_parityComboBox->addItem(tr("偶校验"));
	_parityComboBox->addItem(tr("标志校验"));
	_parityComboBox->addItem(tr("无校验"));
	_parityComboBox->addItem(tr("奇校验"));
	_parityComboBox->addItem(tr("空校验"));
	_parityComboBox->addItem(tr("未知校验"));
	_parityLabel->setBuddy(_parityComboBox);
	connect(_parityComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on__parityComboBox_currentIndexChanged(int)));

	_flowControlLabel = new QLabel(tr("流控制："));
	_flowControlComboBox = new QComboBox;
	_flowControlComboBox->addItem(tr("无流控制"));
	_flowControlComboBox->addItem(tr("硬件流控制"));
	_flowControlComboBox->addItem(tr("软件流控制"));
	_flowControlComboBox->addItem(tr("未知流控制"));
	_flowControlLabel->setBuddy(_flowControlComboBox);
	connect(_flowControlComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on__flowControlComboBox_currentIndexChange(int)));



	if (!isOpen)
	{
		_openCloseButton->setText(tr("打开"));
		flushAvaliablePort();
		_baudRateComboBox->setCurrentIndex(1);
		_dataBitsComboBox->setCurrentIndex(3);
		_stopBitsComboBox->setCurrentIndex(0);
		_parityComboBox->setCurrentIndex(2);
		_flowControlComboBox->setCurrentIndex(1);

	}
	else
	{
		qint32 temp;
		_openCloseButton->setText(tr("关闭"));
		_flushButton->setEnabled(false);
		_portNameComboBox->addItem(serialport->portName());

		temp = serialport->dataBits() - 5;
		if (temp < 0)
		{
			
		}



	}

	
}
