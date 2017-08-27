#ifndef GENERALIZEDDATASTRUCTURE_H_
#define GENERALIZEDDATASTRUCTURE_H_

using uchar = unsigned char;
using Zoom = unsigned short;

template <class T>
class Queue
{
public:
    enum class Status : uchar
    {
        Full = 0x00,
        Empty = 0x01,
        Other = 0x02
    };

    Queue(uchar maxSize = 10)
    {
        size = maxSize;
        p_Data = new T[size];
		flag = Status::Empty;
    }
    bool push(T data)
    {
        if (flag == Status::Full)
            return false;

        p_Data[last++] = data;
        if (last == size)
            last = 0;

        if (last == first)
            flag = Status::Full;
		else
			flag = Status::Other;

        return true;
    }
    T pop()
    {
        if (flag == Status::Empty)
            return nullptr;

        T result = p_Data[first++];
        if (first == size)
            first = 0;

        if (last == first)
            flag = Status::Empty;
		else
			flag = Status::Other;

        return result;
    }
    Status status() { return flag; }

private:
    T *p_Data;
    uchar size;
    uchar first;
    uchar last;
    Status flag;
};

const short MaxSizeOfBuffByte = 1024;

enum class StoreWay : uchar
{
	StoreToSD = 0x01,
	StoreToPC = 0x02
};

enum class BaudRate : uchar
{
    BaudRate9600 = 1,
    BaudRate19200,
    BaudRate38400,
    BaudRate57600,
    BaudRate115200,
    BaudRate230400,
    BaudRate460800,
    BaudRate921600
};

struct DriverInfo
{
    BaudRate baudRate;
    bool hasFirmware;
    bool isSpi;
    uchar softwareAndHardware;
    Zoom zoom;
};

enum class FocusRequest : uchar
{
    AutoFocus = 0x01,
    GetZoom = 0x02,
    SetZoom = 0x03
};

enum class Resolution : uchar
{
    W320 = 1,
    W640,
    W800,
    W1024,
    W1280,
    W1600,
    W2048,
    W2592,
};
enum class Quality : uchar
{
    VeryHigh = 1,
    High,
    Medium,
    Low
};

enum class ImageFormat : uchar
{
    JPG = 1,
    RGB565,
    YUV422
};
enum class Exposure : uchar
{
    AutoExposure = 0x00,
    Exposure0 = 0x80,
    Exposure1 = 0x81,
    Exposure2 = 0x82,
    Exposure3 = 0x83,
    Exposure4 = 0x84,
    Exposure5 = 0x85,
    Exposure6,
    Exposure7,
    Exposure8,
    Exposure9,
    Exposure10,
    Exposure11,
    Exposure12,
    Exposure13,
    Exposure14,
    Exposure15
};

struct TakePictureInfo
{
    Resolution resolution;
    Quality quality;
    bool isColor;
    ImageFormat imageFormat;
    Exposure exposure;
};

enum class ErrorCode : uchar
{
    NoneError = 0x00,
    CommandError = 0x01,
    DriverFaliureError = 0x02,
    BusyError = 0x03,
    CheckError = 0x04,
    DataError = 0x05,
    UnkownError = 0x06
};

struct PicturePackInfo
{
    unsigned int sizeOfByte;
    unsigned short sizeOfPack;
};

class PackBuff
{
  public:
    PackBuff(unsigned short sizeOfBity = 0, unsigned short numberOfPack = 0)
    {
        if (sizeOfBity != 0)
        {
            p_buff = new uchar[sizeOfBity];
        }
        this->sizeOfByte = sizeOfBity;
        this->numberOfPack = numberOfPack;
    }
    uchar *getPointToPackBuff() { return p_buff; }
    ~PackBuff()
    {
        if (p_buff != nullptr)
        {
            delete[] p_buff;
        }
        p_buff = nullptr;
    }

    unsigned short getNumberOfPack() { return numberOfPack; }
    unsigned short getSizeOfByte() { return sizeOfByte; }

  private:
    unsigned short numberOfPack;
    unsigned short sizeOfByte;
    uchar *p_buff = nullptr;
};

#endif
