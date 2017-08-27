#ifndef SYSTEMCONTROLWIDGET_H_
#define SYSTEMCONTROLWIDGET_H_

#include <QWidget>
#include <QDateTime>

class QPushButton;
class QCheckBox;
class QLabel;

class SystemControlWidget :public QWidget
{
	Q_OBJECT
public:
	SystemControlWidget(QWidget *parent = nullptr);

private slots:
	void on__howManyButton_clicked();
	void on__setStoreButton_clicked();
	void on__setTimeButton_clicked();
	void on__timer_timeout();

signals:
	void howManyCameraRequest(const char data);
	void setStoreRequest(const char *data);
	void setTimeReauest(const char *data);

private:
	QLabel *_currentDateTimeLabel;
	QPushButton *_howManyButton;
	QPushButton *_setStoreButton;
	QPushButton *_setTimeButton;
	QCheckBox *_storeToSDCheckBox;
	QCheckBox *_storeToPCCheckBox;

	QTimer *_timer;
};

#endif // !SYSTEMCONTROLWIDGET_H_



