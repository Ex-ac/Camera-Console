#include "Communication.h"
#include <QtCore/QDataStream>
#include <QtCore/QtAlgorithms>


#include <QtCore/QDebug>

#include "../Protocols/Protocols.h"

const int SendBlockMaxSize = 1024 + 13;

SerialPortCommuncation::SerialPortCommuncation(QObject *parent)
    : QObject(parent)
{
    _serialPort.setReadBufferSize(1024 * 1024);
	_lastBuff.resize(1024 * 1024);
    connect(this, SIGNAL(hasPop()), this, SLOT(hasPopSlot()));
}

void SerialPortCommuncation::intervalReadStart(bool y)
{
    if (y && _serialPort.isOpen())
    {
        _timer.start(_intervalMsec);
        connect(&_timer, SIGNAL(timeout()), this, SLOT(on__timer_timeout()));
    }
    else
    {
        _timer.stop();
        disconnect(&_timer, SIGNAL(timeout()), this, SLOT(on__timer_timeout()));
    }
}

QByteArray SerialPortCommuncation::popBuff()
{
    if (_buffQueue.isEmpty())
    {
        return QByteArray();
    }
    QByteArray data;
    _locker.lockForRead();
    data = _buffQueue.first();
    _locker.unlock();

	_locker.lockForWrite();
	_buffQueue.pop_front();
	_locker.unlock();

    return data;
}

void SerialPortCommuncation::sendData(const QByteArray &data)
{
    _serialPort.write(data);
    if (!_serialPort.waitForBytesWritten())
    {
        emit errorHappend(ErrorCode::WriteError);
    }
}

//use to read data
//协议的内容是只有在最后的结束标志是当个的，
//其他都是连续的，结束标志标识一个块数据传输完成，后面接的是开始标志

void SerialPortCommuncation::on__timer_timeout()
{
	
    if (_serialPort.bytesAvailable() == 0)
    {
		return;
    }

	
	_lastBuff.append(_serialPort.readAll());
    

	qDebug() << _lastBuff.toHex();


    QByteArray::iterator begin = _lastBuff.begin();

    while (begin < _lastBuff.end())
    {
        //开头
        QByteArray::iterator temp = qFind(begin, _lastBuff.end(), char(MasterProtocols::Flag::StartFlag));

        if (temp == _lastBuff.end() && *temp != char(MasterProtocols::Flag::StartFlag))
        {
            _lastBuff.clear();
            return;
        }
		
        begin = temp; //不确定

        temp = qFind(begin, _lastBuff.end(), char(MasterProtocols::Flag::EndFlag));

		//qDebug() << char(*begin) << char(*temp);
        if (*temp != char(MasterProtocols::Flag::EndFlag)) //不够长，或错误，
        {
            if ((temp - begin) > SendBlockMaxSize)
            {
                begin = qFind(_lastBuff.end() - SendBlockMaxSize, _lastBuff.end(), char(MasterProtocols::Flag::StartFlag));
                _lastBuff.remove(0, begin - _lastBuff.begin());
            }
            return; //只能是不够长而返回
        }

        //

        QByteArray byteArray;

        int result = checkBuffBlock(begin, temp);
        switch (checkBuffBlock(begin, temp))
        {
        case 0: //noerror
			byteArray.resize(0);
            
			byteArray.append(begin, temp - begin + 1);
			


			_locker.lockForWrite();
			_buffQueue.push_back(byteArray);
            _locker.unlock();

			emit newDataIn(_buffQueue.size());
            begin = temp; //新开始
            break;

        case -1:        //headError

			begin += 1;
            break;

		default: //提前结尾?
            if ((temp + result) < _lastBuff.end())
            {
				temp += result;
				if (*temp == char(MasterProtocols::Flag::EndFlag) && checkBuffBlock(begin, temp) == 0)
				{

					byteArray.resize(0);
					byteArray.append(begin, temp - begin + 1);

					_locker.lockForWrite();
					_buffQueue.push_back(byteArray);
					_locker.unlock();
					emit newDataIn(_buffQueue.size());
					begin = temp; //新开始
				}
                else
                {
					begin += 1;
                }
            }
            else
            {
				_lastBuff.remove(0, begin - _lastBuff.begin());
				return;
                
            }
        }
    }
	_lastBuff.clear();
}

void SerialPortCommuncation::hasPopSlot()
{

}

 int SerialPortCommuncation::checkBuffBlock(QByteArray::iterator & begin, QByteArray::iterator & end)
 {
     if ((end - begin) >= SendBlockMaxSize)
     {
         return -1;
     }

     if (end - begin + 1 == 4)
     {
         return 0;
     }
    
    int count = uchar(*(begin + 1)) * 256 +  uchar(*(begin + 2));

	if (count > SendBlockMaxSize || count < ((end - begin + 1)))
	{
		return -1;
	}
    else if (count == (end - begin + 1))
    {
        //if (MasterProtocols::checksum((const uchar *)(begin + 1), count - 3) == uchar(*(end - 1)))
        //{
        //    return 0;
        //}
        //else
        //{
        //    return -1;
        //}
		return 0;
    }
    else
    {
		return count - (end - begin + 1);
    }
 }