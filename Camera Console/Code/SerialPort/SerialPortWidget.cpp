#include "SerialPortWidget.h"

#include <qpushbutton.h>
#include <qcombobox.h>
#include <qserialportinfo.h>
#include <qlayout.h>
#include <qlist.h>
#include <QLabel>
#include <QSpinBox>

SerialPortWidget::SerialPortWidget(QWidget *parent)
	:QWidget(parent)
{
	openCloseButton = new QPushButton(tr("打开"));
	connect(openCloseButton, SIGNAL(clicked()), this, SLOT(openClosePort()));

	flushButton = new QPushButton(tr("刷新"));
	connect(flushButton, SIGNAL(clicked()), this, SLOT(flushAvaliablePort()));

	portNameLabel = new QLabel(tr("端口名称:"));
	portNameComboBox = new QComboBox;
	portNameLabel->setBuddy(portNameComboBox);
	flushAvaliablePort();

	baudRateLabel = new QLabel(tr("波特率："));
	baudRateComboBox = new QComboBox;
	baudRateComboBox->addItem(tr("4800"));
	baudRateComboBox->addItem(tr("9600"));
	baudRateComboBox->addItem(tr("19200"));
	baudRateComboBox->addItem(tr("38400"));
	baudRateComboBox->addItem(tr("57600"));
	baudRateComboBox->addItem(tr("115200"));
	baudRateComboBox->addItem(tr("Other"));
	baudRateComboBox->addItem(tr("未知"));
	baudRateComboBox->setCurrentIndex(1);

	baudRateLabel->setBuddy(baudRateComboBox);

	connect(baudRateComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on_baudRateComboBox_currentIndexChanged(int)));

	baudRateSpinBox = new QSpinBox;
	baudRateSpinBox->setEnabled(false);
	baudRateSpinBox->setMinimum(0);
	connect(baudRateSpinBox, SIGNAL(valueChanged(int i)), this,
		SLOT(on_baudRateSpinBox_valueChanged()));

	dataBitsLabel = new QLabel(tr("数据位："));
	dataBitsComboBox = new QComboBox;
	dataBitsComboBox->addItem(tr("5"));
	dataBitsComboBox->addItem(tr("6"));
	dataBitsComboBox->addItem(tr("7"));
	dataBitsComboBox->addItem(tr("8"));
	dataBitsComboBox->addItem(tr("未知"));
	dataBitsComboBox->setCurrentIndex(3);

	dataBitsLabel->setBuddy(dataBitsComboBox);
	connect(dataBitsComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(on_dataBitsComboBox_currentIndexChanged(int)));

	stopBitsLabel = new QLabel(tr("结束位："));
	stopBitsComboBox = new QComboBox;
	stopBitsComboBox->addItem(tr("1"));
	stopBitsComboBox->addItem(tr("1.5"));
	

}
