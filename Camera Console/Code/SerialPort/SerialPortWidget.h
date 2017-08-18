#ifndef SERIALPORTWIDGET_H_
#define SERIALPORTWIDGET_H_

/*
	
*/

#include <qwidget.h>
#include <qserialport.h>

class QPushButton;
class QLabel;
class QComboBox;
class QSpinBox;


class SerialPortWidget : public QWidget
{
	Q_OBJECT

public:
	SerialPortWidget(QSerialPort *serialport, QWidget *parent = nullptr);
	


public slots:
	void flushAvaliablePort();
	void openClosePort(QString &name);

private slots:
void on__baudRateComboBox_currentIndexChanged(int i);
void on__baudRateSpinBox_valueChanged(int i);
void on__dataBitsComboBox_currentIndexChanged(int i);
void on__stopBitsComboBox_currentIndexChanged(int i);
void on__parityComboBox_currentIndexChanged(int i);
void on__flowControlComboBox_currentIndexChanged(int i);


signals:
	void message(QString &message);

private:
	QPushButton *_openCloseButton;
	QPushButton *_flushButton;
	
	QLabel *_portNameLabel;
	QComboBox *_portNameComboBox;

	QLabel *_baudRateLabel;
	QComboBox *_baudRateComboBox;
	QSpinBox *_baudRateSpinBox;


	QLabel *_dataBitsLabel;
	QComboBox *_dataBitsComboBox;

	QLabel *_stopBitsLabel;
	QComboBox *_stopBitsComboBox;

	QLabel *_parityLabel;
	QComboBox *_parityComboBox;

	QLabel *_directionLabel;
	QLabel *_directionComboBox;

	QLabel *_flowControlLabel;
	QComboBox *_flowControlComboBox;

	QSerialPort *_serialPort;

	//is need??
	bool isOpened = false;

};


#endif