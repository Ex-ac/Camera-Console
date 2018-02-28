#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtCore/QReadWriteLock>
#include <QtSerialPort/QSerialPort>



class SerialPortCommuncation : public QObject
{
    Q_OBJECT
public:
    enum class ErrorCode
    {
        NoneError,
        ReadError,
        WriteError,
    };

public:
    SerialPortCommuncation(QObject *parent = nullptr);

    void intervalReadStart(bool y = true);
    inline void setInterval(qint64 msec) {_intervalMsec = msec; _timer.setInterval(msec);}
    inline qint64 interval() const {return _intervalMsec;}

    inline QSerialPort &serialPort(){return _serialPort;}

    QByteArray popBuff();
/*
private:
    QByteArray::const_iterator & findStartFlage(QByteArray::const_iterator &begin, QByteArray::const_iterator &end) const;
    QByteArray::const_iterator & findEndFlage(QByteArray::const_iterator &begin, QByteArray::const_iterator &end) const;
*/

public:
    int checkBuffBlock(QByteArray::iterator & begin, QByteArray::iterator & end);

signals:
    void errorHappend(ErrorCode errorCode);
    void newDataIn(int count);
    void hasPop();


public slots:
    void sendData(const QByteArray &data);
    void on__timer_timeout();
    void hasPopSlot();

private:

    QList<QByteArray> _buffQueue;
    QByteArray _lastBuff;
    QSerialPort _serialPort;
    QTimer _timer;
    qint64 _intervalMsec = 100;
    QReadWriteLock _locker;
};


#endif