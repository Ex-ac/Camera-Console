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
	explicit SerialPortWidget(QWidget *parent = nullptr);

public slots:
	void flushAvaliablePort();
	void openClosePort(QString &name);

private slots:
void on_baudRateComboBox_currentIndexChanged(int i);
void on_baudRateSpinBox_valueChanged(int i);
void on_dataBitsComboBox_currentIndexChanged(int i);
signals:
	void message(QString &message);

private:
	QPushButton *openCloseButton;
	QPushButton *flushButton;
	
	QLabel *portNameLabel;
	QComboBox *portNameComboBox;

	QLabel *baudRateLabel;
	QComboBox *baudRateComboBox;
	QSpinBox *baudRateSpinBox;


	QLabel *dataBitsLabel;
	QComboBox *dataBitsComboBox;

	QLabel *stopBitsLabel;
	QComboBox *stopBitsComboBox;

	QLabel *parityLabel;
	QComboBox *partiyComboBox;

	QLabel *directionLabel;
	QLabel *directionComboBox;

	QLabel *flowControlLabel;
	QLabel *floeControlComboBox;

	QSerialPort serialPort;

	//is need??
	bool isOpened = false;

};


#endif