#include "TakePhotoSettingWidget.h"

#include <QLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>

TakePhotoSettingWidget::TakePhotoSettingWidget(QWidget *parent)
	: QWidget(parent)
{
	_takePhotoButton = new QPushButton(tr(u8"拍照"));
	_takePhotoButton->setMinimumHeight(0.5 * _takePhotoButton->sizeHint().width());
	connect(_takePhotoButton, SIGNAL(clicked()), this, SLOT(on__takePhotoButton_clicked()));

	_autoFocusButton = new QPushButton(tr(u8"自动对焦"));
	connect(_autoFocusButton, SIGNAL(clicked()), this, SLOT(on__autoFocusButton_clicked()));


	_setFocusButton = new QPushButton(tr(u8"设置焦距"));
	connect(_setFocusButton, SIGNAL(clicked()), this, SLOT(on__setFocusButton_clicked()));

	_getFocusButton = new QPushButton(tr(u8"读取焦距"));
	connect(_getFocusButton, SIGNAL(clicked()), this, SLOT(on__getFocusButton_clicked()));

	_focusSpinBox = new QSpinBox;
	_focusSpinBox->setMinimum(0);
	_focusSpinBox->setMaximum(750);


	_resolutionLabel = new QLabel(tr(u8"照片分辨率："));
	_resolutionComboBox = new QComboBox;
	_resolutionComboBox->addItem(tr(u8"320×240"), quint8(1));
	_resolutionComboBox->addItem(tr(u8"640×480"), quint8(2));
	_resolutionComboBox->addItem(tr(u8"800×600"), quint8(3));
	_resolutionComboBox->addItem(tr(u8"1024×768"), quint8(4));
	_resolutionComboBox->addItem(tr(u8"1280×960"), quint8(5));
	_resolutionComboBox->addItem(tr(u8"1600×1200"), quint8(6));
	_resolutionComboBox->addItem(tr(u8"2048×1536"), quint8(7));
	_resolutionComboBox->addItem(tr(u8"2592×1944"), quint8(8));
	_resolutionLabel->setBuddy(_resolutionComboBox);

	_qualityLabel = new QLabel(tr(u8"照片质量："));
	_qualityComboBox = new QComboBox;
	_qualityComboBox->addItem(tr(u8"极高"), quint8(1));
	_qualityComboBox->addItem(tr(u8"高"), quint8(2));
	_qualityComboBox->addItem(tr(u8"中等"), quint8(3));
	_qualityComboBox->addItem(tr(u8"低"), quint8(4));
	_qualityLabel->setBuddy(_qualityComboBox);


	_exposureLabel = new QLabel(tr(u8"曝光模式："));
	_exposureComboBox = new QComboBox;
	_exposureComboBox->addItem(tr(u8"自动曝光"), quint8(0x00));
	_exposureComboBox->addItem(tr(u8"+0"), quint8(0x80));
	_exposureComboBox->addItem(tr(u8"+1"), quint8(0x81));
	_exposureComboBox->addItem(tr(u8"+2"), quint8(0x82));
	_exposureComboBox->addItem(tr(u8"+3"), quint8(0x83));
	_exposureComboBox->addItem(tr(u8"+4"), quint8(0x84));
	_exposureComboBox->addItem(tr(u8"+5"), quint8(0x85));
	_exposureComboBox->addItem(tr(u8"+6"), quint8(0x86));
	_exposureComboBox->addItem(tr(u8"+7"), quint8(0x87));
	_exposureComboBox->addItem(tr(u8"+8"), quint8(0x88));
	_exposureComboBox->addItem(tr(u8"+9"), quint8(0x89));
	_exposureComboBox->addItem(tr(u8"+10"), quint8(0x8a));
	_exposureComboBox->addItem(tr(u8"+11"), quint8(0x8b));
	_exposureComboBox->addItem(tr(u8"+12"), quint8(0x8c));
	_exposureComboBox->addItem(tr(u8"+13"), quint8(0x8d));
	_exposureComboBox->addItem(tr(u8"+14"), quint8(0x8e));
	_exposureComboBox->addItem(tr(u8"+15"), quint8(0x8f));
	_exposureLabel->setBuddy(_exposureComboBox);


	_photoColorLabel = new QLabel(tr(u8"照片颜色："));
	_photoColorComboBox = new QComboBox;
	_photoColorComboBox->addItem(tr(u8"黑白"), quint8(0));
	_photoColorComboBox->addItem(tr(u8"彩色"), quint8(1));
	_photoColorLabel->setBuddy(_photoColorComboBox);

	_imageFormatLabel = new QLabel(tr(u8"照片格式："));
	_imageFormatComboBox = new QComboBox;
	_imageFormatComboBox->addItem(tr(u8"JPEG"), quint8(1));
	_imageFormatComboBox->addItem(tr(u8"RGB565"), quint8(2));
	_imageFormatComboBox->addItem(tr(u8"YUV422"), quint8(3));
	_imageFormatLabel->setBuddy(_imageFormatComboBox);

	QGroupBox *takePhotoGroupBox = new QGroupBox(tr(u8"拍照设置"));
	QGridLayout *takePhotoLayout = new QGridLayout;


	takePhotoLayout->addWidget(_resolutionLabel, 0, 0, 1, 2);
	takePhotoLayout->addWidget(_qualityLabel, 0, 2);
	takePhotoLayout->addWidget(_resolutionComboBox, 1, 0, 1, 2);
	takePhotoLayout->addWidget(_qualityComboBox, 1, 2);
	takePhotoLayout->addWidget(_exposureLabel, 2, 0);
	takePhotoLayout->addWidget(_photoColorLabel, 2, 1);
	takePhotoLayout->addWidget(_imageFormatLabel, 2, 2);
	takePhotoLayout->addWidget(_exposureComboBox, 3, 0);
	takePhotoLayout->addWidget(_photoColorComboBox, 3, 1);
	takePhotoLayout->addWidget(_imageFormatComboBox, 3, 2);
	takePhotoLayout->addWidget(_takePhotoButton, 1, 3, 2, 2, Qt::AlignmentFlag::AlignVCenter);
	takePhotoGroupBox->setLayout(takePhotoLayout);

	QGroupBox *focusGroupBox = new QGroupBox(tr(u8"焦距设置"));
	QGridLayout *focusLayout = new QGridLayout;
	focusLayout->addWidget(_autoFocusButton, 0, 0);
	focusLayout->addWidget(_getFocusButton, 0, 1);
	focusLayout->addWidget(_focusSpinBox, 1, 0);
	focusLayout->addWidget(_setFocusButton, 1, 1);
	focusGroupBox->setLayout(focusLayout);


	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(takePhotoGroupBox);
	mainLayout->addWidget(focusGroupBox);
	setLayout(mainLayout);
}

void TakePhotoSettingWidget::on__takePhotoButton_clicked()
{
	TakePictureInfo info;
	info.resolution = Resolution(_resolutionComboBox->currentData().toUInt());
	info.quality = Quality(_qualityComboBox->currentData().toUInt());
	info.isColor = _photoColorComboBox->currentData().toUInt() > 0 ? true : false;
	info.imageFormat = ImageFormat(_imageFormatComboBox->currentData().toUInt());
	info.exposure = Exposure(_exposureComboBox->currentData().toUInt());

	uchar data[RequestCommandBuffSize] = { 0x00 };

	MasterProtocols::takePictureRequest(&data, &info);
	emit request((const char *)data);
}

void TakePhotoSettingWidget::on__autoFocusButton_clicked()
{
	FocusRequest request = FocusRequest::AutoFocus;
	uchar data[RequestCommandBuffSize] = { 0x00 };
	MasterProtocols::focusRequest(&data, request);
	emit TakePhotoSettingWidget::request((const char *)data);
}

void TakePhotoSettingWidget::on__setFocusButton_clicked()
{
	FocusRequest request = FocusRequest::SetZoom;
	uchar data[RequestCommandBuffSize] = { 0x00 };
	MasterProtocols::focusRequest(&data, request, _focusSpinBox->value());
	emit TakePhotoSettingWidget::request((const char *)data);
}

void TakePhotoSettingWidget::on__getFocusButton_clicked()
{
	FocusRequest request = FocusRequest::GetZoom;
	uchar data[RequestCommandBuffSize] = { 0x00 };
	MasterProtocols::focusRequest(&data, request);
	emit TakePhotoSettingWidget::request((const char *)data);
}
