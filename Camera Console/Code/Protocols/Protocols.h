#ifndef PROTOCLOS_H_
#define PROTOCLOS_H_

#include "generalizeddatastructure.h"


const uchar AnswerCommandBuffSize = 13;
const uchar RequestCommandBuffSize = 7;

class AbstractProtocolsTransfers
{
  public:
    AbstractProtocolsTransfers(uchar id = 0) : driverId(id) {}
    uchar driverId;
    uchar receiveDataBuff[AnswerCommandBuffSize];
    uchar sendDataBuff[RequestCommandBuffSize];
    PackBuff *p_PackBuff = nullptr;
	
    virtual void sendRequestCommand(uchar (*data)[RequestCommandBuffSize]);
    virtual void sendByte(uchar data) = 0;
    virtual uchar getByte() = 0;
    virtual void getLongByte(uchar *buff, unsigned short size) = 0;
    virtual void getFirst11ByteCommand();
    virtual void getLast2ByteCommand();
	
};

class MasterProtocols
{
  public:
    enum class Flag : uchar
    {
        StartFlag = 0x7e,
        EndFlag = 0xe7
    };
    enum class RequestCommand : uchar
    {
        Inquiry = 0x01,
        Focus = 0x09,
        TakePicture = 0x05,
        GetPicturePack = 0x07
    };
    enum class AnswerCommand : uchar
    {
        Inquiry = 0x02,
        Focus = 0x0a,
        TakePicture = 0x06,
        GetPicturePack = 0x08
    };
    static unsigned short packSize(AbstractProtocolsTransfers
                                       *p_transfers);
    static uchar driverId(AbstractProtocolsTransfers *p_transfers);
    static uchar packAnswerCommand(AbstractProtocolsTransfers
                                       *p_transfers);
    static void inquiryRequest(uchar (*data)[RequestCommandBuffSize]);
    static void focusRequest(uchar (*data)[RequestCommandBuffSize], const FocusRequest focus,
                             const Zoom zoom = 0);
    static void takePictureRequest(uchar (*data)[RequestCommandBuffSize],
                                   const TakePictureInfo *p_info);
    static void getPicturePackRequest(uchar (*data)[RequestCommandBuffSize],
                                      const unsigned short numberOfPack, const PicturePackInfo *p_info);
    static bool packCheck(AbstractProtocolsTransfers *p_transfers);
    static uchar checksum(const uchar(*data), const unsigned short count);
    static ErrorCode inquiryAnswer(AbstractProtocolsTransfers
                                       *p_transfers,
                                   DriverInfo *p_info);
    static ErrorCode takePictureAnswer(AbstractProtocolsTransfers
                                           *p_transfers,
                                       PicturePackInfo *p_info);
    static ErrorCode foucsAnswer(AbstractProtocolsTransfers
                                     *p_transfers,
                                 Zoom *p_zoom);
    static ErrorCode getPicturePackAnswer(AbstractProtocolsTransfers
                                              *p_transfers,
                                          const PicturePackInfo *p_info);
};

#endif
