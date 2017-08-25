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

	void setImage(const QImage &image);
	void setZoom(Zoom zoom) { _zoom = zoom; }
	Zoom zoom() { return _zoom; }
	const PicturePackInfo &picturePackInfo() { return _picturePackInfo; }
	void setPicturePackInfo(const PicturePackInfo &info) { _picturePackInfo = info; }
	void setCurrentPack(int current) { _currentPack = current; }
	int currentPack() { return _currentPack; }

	//void dealDataPack(PackBuff *packBuff);

protected:
	//void hint();

private:
	const int _id;
	Zoom _zoom;
	PicturePackInfo _picturePackInfo;
	int _currentPack;
	QImage _image;

	QLabel *_zoomTitleLabel;
	QLabel *_zoomValueLabel;
	QLabel *_sizeOfByteTiletLabel;
	QLabel *_sizeOfByteValueLanel;
	QLabel *_sizeOfPackTitleLabel;
	QLabel *_sizeOfPackValueLabel;
	QLabel *_currentPackTitleLabel;
	QLabel *_currentPackValueLabel;

};

#endif
