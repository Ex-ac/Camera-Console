#include "CameraWidget.h"

#include <QtWidgets/QLabel>
#include <QtGui/QPalette>
#include <QtWidgets/QLayout>
#include <QtGui/QPixmap>
#include <QtCore/QDir>
#include <QtWidgets/QPushButton>
#include <QtCore/QDebug>

#include <QtCore/QFile>

CameraWidget::CameraWidget(int id, QWidget *parent)
	: QWidget(parent), _id(id)
{
	this->setWindowTitle(tr(u8"相机%1").arg(_id));

	setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground, false);

	_titleLabel = new QLabel(tr(u8"相机--%1").arg(_id));
	setStyleSheet("color:red;");
	_zoomTitleLabel = new QLabel(tr(u8"焦距："));
	_zoomValueLabel = new QLabel(tr(u8"待获取"));

	_sizeOfByteTiletLabel = new QLabel(tr(u8"图片大小："));
	_sizeOfByteValueLabel = new QLabel(tr(u8"待拍照"));

	_sizeOfPackTitleLabel = new QLabel(tr(u8"包数量："));
	_sizeOfPackValueLabel = new QLabel(tr(u8"待拍照"));

	_currentPackTitleLabel = new QLabel(tr(u8"当前包："));
	_currentPackValueLabel = new QLabel(tr(u8"待获取"));

	_storeWayTitleLabel = new QLabel(tr(u8"存储方式："));
	_storeWayValueLabel = new QLabel(tr(u8"待设置"));

	_statusTitleLabel = new QLabel(tr(u8"状态："));
	_statusValueLabel = new QLabel(tr(u8"空闲"));

	QVBoxLayout *mainLayout = new QVBoxLayout;

	QHBoxLayout *tempLayout = new QHBoxLayout;

	mainLayout->addWidget(_titleLabel, 0, Qt::AlignmentFlag::AlignHCenter);

	tempLayout->addWidget(_zoomTitleLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addWidget(_zoomValueLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addStretch(100);
	mainLayout->addLayout(tempLayout);

	tempLayout = new QHBoxLayout;
	tempLayout->addWidget(_sizeOfByteTiletLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addWidget(_sizeOfByteValueLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addStretch(100);
	mainLayout->addLayout(tempLayout);

	tempLayout = new QHBoxLayout;
	tempLayout->addWidget(_sizeOfPackTitleLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addWidget(_sizeOfPackValueLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addStretch(100);
	mainLayout->addLayout(tempLayout);

	tempLayout = new QHBoxLayout;
	tempLayout->addWidget(_currentPackTitleLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addWidget(_currentPackValueLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addStretch(100);
	mainLayout->addLayout(tempLayout);

	tempLayout = new QHBoxLayout;
	tempLayout->addWidget(_storeWayTitleLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addWidget(_storeWayValueLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addStretch(100);

	mainLayout->addStretch(100);

	tempLayout = new QHBoxLayout;
	tempLayout->addWidget(_statusTitleLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addWidget(_statusValueLabel, 0, Qt::AlignmentFlag::AlignLeft);
	tempLayout->addStretch(100);
	mainLayout->addLayout(tempLayout);

	setAutoFillBackground(true);

	setLayout(mainLayout);
}

void CameraWidget::setImage(const QString &fileName)
{
	QPalette backgroundPalette = this->palette();

	QFile file(fileName);
	file.open(QIODevice::OpenModeFlag::WriteOnly);


	file.write(_byteArray);
	file.close();

	backgroundPalette.setBrush(QPalette::Background,
							   QPixmap(fileName).scaled(this->size(), Qt::AspectRatioMode::KeepAspectRatioByExpanding));
	setPalette(backgroundPalette);
}

void CameraWidget::setZoom(Zoom zoom)
{
	_zoom = zoom;
	_zoomValueLabel->setText(tr(u8"%1").arg(zoom));
}

void CameraWidget::setPicturePackInfo(const PicturePackInfo &info)
{
	_packBitArray.resize(info.sizeOfPack);

	_packBitArray.fill(false);

	_isAllPackHasGet = false;
	_firstGetFinished = false;

	_byteArray.resize(info.sizeOfByte);

	qDebug() << "ID: " << _id << "\t Byte: " << info.sizeOfByte << "\t Pack:" << info.sizeOfPack;

	_picturePackInfo = info;
	_sizeOfByteValueLabel->setText(tr(u8"%1").arg(info.sizeOfByte));
	_sizeOfPackValueLabel->setText(tr(u8"%1").arg(info.sizeOfPack));
}

inline void CameraWidget::setCurrentPack(int current)
{
	_currentPack = current;
	_currentPackValueLabel->setText(tr(u8"%1").arg(current));
}

inline void CameraWidget::setStoreWay(StoreWay storeWay)
{
	_storeWay = storeWay;
	QString string;

	if (uchar(storeWay) & uchar(StoreWay::StoreToPC))
	{
		string += tr(u8"电脑 ");
	}
	if (uchar(storeWay) & uchar(StoreWay::StoreToSD))
	{
		string += tr(u8"存储卡 ");
	}
	_storeWayValueLabel->setText(string);
}

void CameraWidget::dealDataPack(const QByteArray &data)
{

	unsigned short length = uchar(data[1]) * 256 + uchar(data[2]);
	unsigned short numberOfPack = uchar(data[6]) * 256 + uchar(data[7]);

	

	qCopy(data.begin() + 11, data.end() - 2,
		  _byteArray.begin() + (1024 * (numberOfPack - 1)));

	qDebug() << "ID: " << _id << "\t Pack:" << numberOfPack - 1 << "\tByte:" << length << "\tSize: " << _packBitArray.size();
	_packBitArray.setBit(numberOfPack - 1, true);
	
	

	setCurrentPack(numberOfPack);


	if (!_firstGetFinished)
	{
		if (numberOfPack == _picturePackInfo.sizeOfPack)
		{
			_firstGetFinished = true;
		}
	}

	if (!_firstGetFinished)
	{
		return;
	}

	if (isAllPackHasGet())
	{
		QString fileName = QDir::currentPath() + tr("/%1.jpg").arg(_id);
		setImage(fileName);
	}
	else
	{
		emit hasPackNotGet();
	}
}

QList<unsigned short> CameraWidget::packNeedGet()
{
	QList<unsigned short> data;

	for (int i = 0; i < _packBitArray.size(); ++i)
	{
		if (!_packBitArray.at(i))
		{
			data.append(i + 1);
		}
	}

	return data;
}

bool CameraWidget::isAllPackHasGet()
{
	if (_isAllPackHasGet)
	{
		return true;
	}

	for (int i = 0; i < _packBitArray.size(); ++i)
	{
		if (!_packBitArray.at(i))
		{
			return false;
		}
	}
	_isAllPackHasGet = true;
	return _isAllPackHasGet;
}
