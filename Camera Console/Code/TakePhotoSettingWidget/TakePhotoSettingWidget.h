#ifndef TAKEPHOTOSETTINGWIDGET_H_
#define TAKEPHOTOSETTINGWIDGET_H_

#include <QWidget>
#include "Code\Protocols\Protocols.h"
#include "Code\Protocols\generalizeddatastructure.h"

class QPushButton;
class QSpinBox;
class QLabel;
class QComboBox;


class TakePhotoSettingWidget : public QWidget
{
	Q_OBJECT
	
public:
	TakePhotoSettingWidget(QWidget *parent = nullptr);
signals:
	void request(const char *data);
	
private slots:
	void on__takePhotoButton_clicked();
	void on__autoFocusButton_clicked();
	void on__setFocusButton_clicked();
	void on__getFocusButton_clicked();

private:
	QPushButton *_takePhotoButton;
	QPushButton *_autoFocusButton;
	QPushButton *_setFocusButton;
	QPushButton *_getFocusButton;
	QSpinBox *_focusSpinBox;

	QLabel *_resolutionLabel;
	QComboBox *_resolutionComboBox;

	QLabel *_qualityLabel;
	QComboBox *_qualityComboBox;

	QLabel *_imageFormatLabel;
	QComboBox *_imageFormatComboBox;

	QLabel *_exposureLabel;
	QComboBox *_exposureComboBox;

	QLabel *_photoColorLabel;
	QComboBox *_photoColorComboBox;
};


#endif // !TAKEPHOTOSETTINGWIDGET_H_
