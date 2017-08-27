#ifndef CAMERAWIDGET_H_
#define CAMERAWIDGET_H_

#include <QDialog>
#include <QImage>


#include "Code\Protocols\generalizeddatastructure.h"
#include "Code\Protocols\Protocols.h"


class QLabel;

class CameraWidget : public QWidget
{
	Q_OBJECT

public:
	CameraWidget(int id, QWidget *parent = nullptr);

	void setZoom(Zoom zoom);
	inline unsigned short zoom() { return _zoom; }


	inline const PicturePackInfo &picturePackInfo() { return _picturePackInfo; }
	void setPicturePackInfo(const PicturePackInfo &info);


	void setCurrentPack(int current);
	inline int currentPack() { return _currentPack; }

	inline StoreWay storeWay() { return _storeWay; }
	void setStoreWay(StoreWay storeWay);

	inline bool needStoreToPC() { return uchar(_storeWay) & uchar(StoreWay::StoreToPC); }

	void dealDataPack(PackBuff &packBuff);
	void setImage(const QString &fileName);

private:
	const int _id;
	Zoom _zoom;
	PicturePackInfo _picturePackInfo;
	int _currentPack;
	QByteArray _byteArray;
	StoreWay _storeWay;

	QLabel *_titleLabel;
	QLabel *_zoomTitleLabel;
	QLabel *_zoomValueLabel;
	QLabel *_sizeOfByteTiletLabel;
	QLabel *_sizeOfByteValueLabel;
	QLabel *_sizeOfPackTitleLabel;
	QLabel *_sizeOfPackValueLabel;
	QLabel *_currentPackTitleLabel;
	QLabel *_currentPackValueLabel;

	QLabel *_storeWayTitleLabel;
	QLabel *_storeWayValueLabel;
	QLabel *_statusTitleLabel;
	QLabel *_statusValueLabel;
};

#endif
