#include "SystemControlWidget.h"

#include <QPushButton>

#include <QDateTimeEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QLayout>
#include <QByteArray>
#include <qtimer.h>
#include <QLabel>

SystemControlWidget::SystemControlWidget(QWidget *parent)
	:QWidget(parent)
{

	_currentDateTimeLabel = new QLabel;
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(on__timer_timeout()));
	_timer->start(500);


	_setTimeButton = new QPushButton(tr(u8"设置时间"));
	connect(_setTimeButton, SIGNAL(clicked()), this, SLOT(on__setTimeButton_clicked()));

	_howManyButton = new QPushButton(tr(u8"查询相机数量"));
	connect(_howManyButton, SIGNAL(clicked()), this, SLOT(on__howManyButton_clicked()));


	_storeToSDCheckBox = new QCheckBox(tr(u8"存到存储卡中"));
	_storeToPCCheckBox = new QCheckBox(tr(u8"发送打电脑"));
	_setStoreButton = new QPushButton(tr(u8"设置存储方式"));
	connect(_setStoreButton, SIGNAL(clicked()), this, SLOT(on__setStoreButton_clicked()));

	QGroupBox *mainGroupBox = new QGroupBox(tr(u8"系统设置"));

	QGridLayout *groupLayout = new QGridLayout;
	groupLayout->addWidget(_currentDateTimeLabel, 0, 0, 1, 3, Qt::AlignmentFlag::AlignHCenter);
	groupLayout->addWidget(_howManyButton, 1, 0);
	groupLayout->addWidget(_setTimeButton, 1, 2);
	groupLayout->addWidget(_storeToSDCheckBox, 2, 0);
	groupLayout->addWidget(_storeToPCCheckBox, 2, 1);
	groupLayout->addWidget(_setStoreButton, 2, 2);

	mainGroupBox->setLayout(groupLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mainGroupBox);
	setLayout(mainLayout);
}

void SystemControlWidget::on__setStoreButton_clicked()
{
	char data[2] = { 0x02 };
	if (_storeToSDCheckBox->isChecked())
	{
		data[1] |= 0x01;
	}
	if (_storeToPCCheckBox->isChecked())
	{
		data[1] |= 0x02;
	}
	emit setStoreRequest(data);
}

void SystemControlWidget::on__setTimeButton_clicked()
{
	char data[5] = { 0x03 };
	QDateTime dateTime(QDateTime::currentDateTime());
	uint temp = dateTime.toTime_t();
	for (int i = 1; i < 5; ++i)
	{
		data[i] = temp & (0xff << (2 * (5 - i)));
	}
	emit setTimeReauest(data);
}

void SystemControlWidget::on__howManyButton_clicked()
{
	emit howManyCameraRequest(0x01);
}

void SystemControlWidget::on__timer_timeout()
{

	_currentDateTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd   HH: mm: ss"));
}