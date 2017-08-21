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

	inline QSerialPort *_currentSerialPort() { return _serialPort; }

signals:
	void message(const QString &str);

public slots:
	void flushAvaliablePort();
	void openClosePort();

private slots:
	void on__baudRateComboBox_currentIndexChanged(int);
	void on__baudRateSpinBox_editingFinished();
	void on__dataBitsComboBox_currentIndexChanged(int);
	void on__stopBitsComboBox_currentIndexChanged(int);
	void on__parityComboBox_currentIndexChanged(int);
	void on__flowControlComboBox_currentIndexChanged(int);


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


	QLabel *_flowControlLabel;
	QComboBox *_flowControlComboBox;


	QLabel *_directionLabel;
	QComboBox *_directionComboBox;


	QSerialPort *_serialPort;


};


#endif