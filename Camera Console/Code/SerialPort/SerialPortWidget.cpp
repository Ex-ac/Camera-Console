#include "SerialPortWidget.h"

#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qcombobox.h>
#include <QtSerialPort/qserialportinfo.h>
#include <QtWidgets/qlayout.h>
#include <QtCore/qlist.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

SerialPortWidget::SerialPortWidget(QSerialPort *serialport, QWidget *parent)
	:QWidget(parent), _serialPort(serialport)
{
	bool isOpen = _serialPort->isOpen();

	_openCloseButton = new QPushButton;
	_openCloseButton->setDefault(true);

	_flushButton = new QPushButton(tr(u8"刷新"));
	

	_portNameLabel = new QLabel(tr(u8"端口名称:"));
	_portNameComboBox = new QComboBox;
	_portNameLabel->setBuddy(_portNameComboBox);
	

	_baudRateLabel = new QLabel(tr(u8"波特率："));
	_baudRateComboBox = new QComboBox;
	_baudRateComboBox->addItem(tr("4800"), 4800);
	_baudRateComboBox->addItem(tr("9600"), 9600);
	_baudRateComboBox->addItem(tr("19200"), 19200);
	_baudRateComboBox->addItem(tr("38400"), 38400);
	_baudRateComboBox->addItem(tr("57600"), 57600);
	_baudRateComboBox->addItem(tr("115200"), 11520);
	_baudRateComboBox->addItem(tr(u8"未知"), -1);
	_baudRateLabel->setBuddy(_baudRateComboBox);

	

	_baudRateSpinBox = new QSpinBox;
	
	_baudRateSpinBox->setMinimum(0);
	_baudRateSpinBox->setMaximum(99999999);
	_baudRateSpinBox->setValue(1152000);
	//_baudRateSpinBox->setMinimumWidth(70);
	_baudRateSpinBox->setEnabled(false);
	
	_dataBitsLabel = new QLabel(tr(u8"数据位："));
	_dataBitsComboBox = new QComboBox;
	_dataBitsComboBox->addItem(tr("5"),5);
	_dataBitsComboBox->addItem(tr("6"), 6);
	_dataBitsComboBox->addItem(tr("7"), 7);
	_dataBitsComboBox->addItem(tr("8"), 8);
	_dataBitsComboBox->addItem(tr(u8"未知"), -1);
	

	_dataBitsLabel->setBuddy(_dataBitsComboBox);
	

	_stopBitsLabel = new QLabel(tr(u8"结束位："));
	_stopBitsComboBox = new QComboBox;
	_stopBitsComboBox->addItem(tr("1"), 1);
	_stopBitsComboBox->addItem(tr("2"), 2);
	_stopBitsComboBox->addItem(tr("1.5"), 3);
	_stopBitsComboBox->addItem(tr(u8"未知"), -1);
	
	_stopBitsLabel->setBuddy(_stopBitsComboBox);
	

	_parityLabel = new QLabel(tr(u8"校验位："));
	_parityComboBox = new QComboBox;
	_parityComboBox->addItem(tr(u8"无校验"), 0);
	_parityComboBox->addItem(tr(u8"偶标志"), 2);
	_parityComboBox->addItem(tr(u8"奇校验"), 3);
	_parityComboBox->addItem(tr(u8"空格校验"), 4);
	_parityComboBox->addItem(tr(u8"标志校验"), 5);
	_parityComboBox->addItem(tr(u8"未知校验"), -1);
	_parityLabel->setBuddy(_parityComboBox);
	

	_flowControlLabel = new QLabel(tr(u8"流控制："));
	_flowControlComboBox = new QComboBox;
	_flowControlComboBox->addItem(tr(u8"无流控制"), 0);
	_flowControlComboBox->addItem(tr(u8"硬件流控制"), 1);
	_flowControlComboBox->addItem(tr(u8"软件流控制"), 2);
	_flowControlComboBox->addItem(tr(u8"未知流控制"), -1);
	_flowControlLabel->setBuddy(_flowControlComboBox);
	


	_directionLabel = new QLabel(tr(u8"传输方向："));
	_directionComboBox = new QComboBox;
	_directionComboBox->addItem(tr(u8"输入"), QSerialPort::Direction::Input);
	_directionComboBox->addItem(tr(u8"输出"), QSerialPort::Direction::Output);
	_directionComboBox->addItem(tr(u8"输入/输出"), QSerialPort::Direction::AllDirections);


	//设置串口的初始值
	if (!isOpen)
	{
		_openCloseButton->setText(tr(u8"打开"));
		flushAvaliablePort();
		_baudRateComboBox->setCurrentIndex(5);
		_baudRateSpinBox->setEnabled(false);
		_dataBitsComboBox->setCurrentIndex(3);
		_stopBitsComboBox->setCurrentIndex(0);
		_parityComboBox->setCurrentIndex(0);
		_flowControlComboBox->setCurrentIndex(0);
		_directionComboBox->setCurrentIndex(2);

	}
	else
	{
		qint32 temp;
		_openCloseButton->setText(tr(u8"关闭"));
		_flushButton->setEnabled(false);
		_portNameComboBox->addItem(serialport->portName());
		_portNameComboBox->setEnabled(false);
		_baudRateSpinBox->setEnabled(false);
		temp = _serialPort->baudRate();
		switch (temp)
		{
		case QSerialPort::BaudRate::Baud4800:
			_baudRateComboBox->setCurrentIndex(0);
			break;

		case QSerialPort::BaudRate::Baud9600:
			_baudRateComboBox->setCurrentIndex(1);
			break;

		case QSerialPort::BaudRate::Baud19200:
			_baudRateComboBox->setCurrentIndex(2);
			break;

		case QSerialPort::BaudRate::Baud38400:
			_baudRateComboBox->setCurrentIndex(3);
			break;

		case QSerialPort::BaudRate::Baud57600:
			_baudRateComboBox->setCurrentIndex(4);
			break;

		case QSerialPort::BaudRate::Baud115200:
			_baudRateComboBox->setCurrentIndex(5);
			break;


		default:
			_baudRateComboBox->setCurrentIndex(6);
			_baudRateSpinBox->setEnabled(true);
			_baudRateSpinBox->setValue(temp);
			break;
		}

		switch (_serialPort->dataBits())
		{
		case QSerialPort::DataBits::UnknownDataBits:
			_dataBitsComboBox->setCurrentIndex(4);
			break;

		default:
			_dataBitsComboBox->setCurrentIndex(_serialPort->dataBits() - 5);
			break;
		}

		if (_serialPort->flowControl() == QSerialPort::FlowControl::UnknownFlowControl)
		{
			_flowControlComboBox->setCurrentIndex(3);
		}
		else
		{
			_flowControlComboBox->setCurrentIndex(_serialPort->flowControl());
		}

		switch (_serialPort->parity())
		{
		case QSerialPort::Parity::NoParity:
			_parityComboBox->setCurrentIndex(0);
			break;

		case QSerialPort::Parity::UnknownParity:
			_parityComboBox->setCurrentIndex(5);
		default:
			_parityComboBox->setCurrentIndex(_serialPort->flowControl() - 1);
			break;
		}

		switch (_serialPort->stopBits())
		{
		case QSerialPort::StopBits::UnknownStopBits:
			_stopBitsComboBox->setCurrentIndex(3);

		default:
			_stopBitsComboBox->setCurrentIndex(_serialPort->stopBits() - 1);
		}
		_directionComboBox->setEnabled(false);
		_directionComboBox->setCurrentIndex(2);
		
	}


	connect(_openCloseButton, SIGNAL(clicked()), this, SLOT(openClosePort()));
	connect(_flushButton, SIGNAL(clicked()), this, SLOT(flushAvaliablePort()));
	connect(_baudRateComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on__baudRateComboBox_currentIndexChanged(int)));
	connect(_baudRateSpinBox, SIGNAL(editingFinished()), this,
		SLOT(on__baudRateSpinBox_editingFinished()));
	connect(_dataBitsComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on__dataBitsComboBox_currentIndexChanged(int)));
	connect(_stopBitsComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on__stopBitsComboBox_currentIndexChanged(int)));
	connect(_parityComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on__parityComboBox_currentIndexChanged(int)));
	connect(_flowControlComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on__flowControlComboBox_currentIndexChanged(int)));
	//布局
	QGridLayout *topLayout = new QGridLayout;
	topLayout->addWidget(_portNameLabel, 0, 0);
	topLayout->addWidget(_portNameComboBox, 0, 1, 1, 2);
	topLayout->addWidget(_baudRateLabel, 1, 0);
	topLayout->addWidget(_baudRateComboBox, 1, 1);
	topLayout->addWidget(_baudRateSpinBox, 1, 2);
	topLayout->addWidget(_dataBitsLabel, 2, 0);
	topLayout->addWidget(_dataBitsComboBox, 2, 1);
	topLayout->addWidget(_stopBitsLabel, 3, 0);
	topLayout->addWidget(_stopBitsComboBox, 3, 1);
	topLayout->addWidget(_parityLabel, 4, 0);
	topLayout->addWidget(_parityComboBox, 4, 1);
	topLayout->addWidget(_flowControlLabel, 5, 0);
	topLayout->addWidget(_flowControlComboBox, 5, 1);

	topLayout->addWidget(_directionLabel, 6, 0);
	topLayout->addWidget(_directionComboBox, 6, 1);

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(_openCloseButton);
	bottomLayout->addStretch();
	bottomLayout->addWidget(_flushButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);

	setLayout(mainLayout);

}


void SerialPortWidget::flushAvaliablePort()
{
	QList<QSerialPortInfo> avaliableSerialPortList = QSerialPortInfo::availablePorts();

	_portNameComboBox->clear();
	for (auto i : avaliableSerialPortList)
	{
		_portNameComboBox->addItem(i.portName());
	}

}

void SerialPortWidget::openClosePort()
{
	bool isOpen = _serialPort->isOpen();
	
	if (isOpen) 
	{
		//运行关闭操作
		_openCloseButton->setText(tr(u8"打开"));
		_flushButton->setEnabled(true);
		_portNameComboBox->setEnabled(true);
		_directionComboBox->setEnabled(true);

		_serialPort->close();
		flushAvaliablePort();
		emit hasOpen(false);
	}
	else
	{
		_serialPort->setPortName(_portNameComboBox->currentText());
		if (_baudRateComboBox->currentData().toInt() == -1)
		{
			_serialPort->setBaudRate(qint32(_baudRateSpinBox->value()));
		}
		else
		{
			_serialPort->setBaudRate(qint32(_baudRateComboBox->currentData().toInt()));
		}

		_serialPort->setStopBits(QSerialPort::StopBits(_stopBitsComboBox->currentData().toInt()));
		_serialPort->setDataBits(QSerialPort::DataBits(_dataBitsComboBox->currentData().toInt()));
		_serialPort->setParity(QSerialPort::Parity(_parityComboBox->currentData().toInt()));
		_serialPort->setFlowControl(
			QSerialPort::FlowControl(_flowControlComboBox->currentData().toInt()));
		
		if (_serialPort->open((QIODevice::OpenMode(_directionComboBox->currentData().toUInt()))))
		{
			_openCloseButton->setText(u8"关闭");
			_flushButton->setEnabled(false);
			_portNameComboBox->setEnabled(false);
			_directionComboBox->setEnabled(false);
			emit hasOpen(true);
		}
		else
		{
			emit hasOpen(false);
		}
		_serialPort->setRequestToSend(true);
		_serialPort->setDataTerminalReady(false);
	}
}

void SerialPortWidget::on__baudRateComboBox_currentIndexChanged(int i)
{
	if (_baudRateComboBox->currentData().toInt() == -1)
	{
		_baudRateSpinBox->setEnabled(true);
	}
	else
	{
		_baudRateSpinBox->setEnabled(false);
		_serialPort->setBaudRate(qint32(_baudRateComboBox->currentData().toInt()));
	}
}

void SerialPortWidget::on__baudRateSpinBox_editingFinished()
{
	//逻辑上没问题
	_serialPort->setBaudRate(qint32(_baudRateSpinBox->value()));
}

void SerialPortWidget::on__dataBitsComboBox_currentIndexChanged(int i)
{
	_serialPort->setDataBits(QSerialPort::DataBits(_dataBitsComboBox->currentData().toInt()));
}

void SerialPortWidget::on__stopBitsComboBox_currentIndexChanged(int i)
{
	_serialPort->setStopBits(QSerialPort::StopBits(_stopBitsComboBox->currentData().toInt()));
}

void SerialPortWidget::on__parityComboBox_currentIndexChanged(int i)
{
	_serialPort->setParity(QSerialPort::Parity(_parityComboBox->currentData().toInt()));
}

void SerialPortWidget::on__flowControlComboBox_currentIndexChanged(int i)
{
	_serialPort->setFlowControl(QSerialPort::FlowControl(_flowControlComboBox->currentData().toInt()));
}