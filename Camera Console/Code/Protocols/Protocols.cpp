#include "Protocols.h"

#include <stdio.h>

void AbstractProtocolsTransfers::sendRequestCommand(uchar (*data)[RequestCommandBuffSize])
{
    uchar sendData[AnswerCommandBuffSize] = {uchar(MasterProtocols::Flag::StartFlag), 0x00, 0x08, driverId};
    for (int i = 0; i < RequestCommandBuffSize; ++i)
    {
        sendData[i + 4] = (*data)[i];
        sendDataBuff[i] = (*data)[i];
    }
    sendData[11] = MasterProtocols::checksum(sendData + 1, AnswerCommandBuffSize - 3);
    sendData[12] = uchar(MasterProtocols::Flag::EndFlag);

    //debug
    //for (int i = 0; i < AnswerCommandBuffSize; ++i)
    //{
    //    printf("%2x ", sendData[i]);
    //}
    //printf("\n");

    for (int i = 0; i < AnswerCommandBuffSize; ++i)
    {
        sendByte(sendData[i]);
    }
}

void AbstractProtocolsTransfers::getFirst11ByteCommand()
{
    bool isStartFlag = false;
    for (int i = 0; i < AnswerCommandBuffSize - 2;)
    {
        receiveDataBuff[i] = getByte();
        if (isStartFlag || receiveDataBuff[0] == uchar(MasterProtocols::Flag::StartFlag))
        {
            ++i;
        }
        if (receiveDataBuff[1] == uchar(MasterProtocols::Flag::StartFlag))
        {
            i = 1;
            //			printf("getFirst11ByteError\n");
        }
    }
}

void AbstractProtocolsTransfers::getLast2ByteCommand()
{
    for (int i = AnswerCommandBuffSize - 2; i < AnswerCommandBuffSize; ++i)
    {
        receiveDataBuff[i] = getByte();
    }

    //for (uchar i = 0; i < AnswerCommandBuffSize; ++i)
    //{
    //    printf("%2x ", receiveDataBuff[i]);
    //}
    //printf("\n");
}

unsigned short MasterProtocols::packSize(AbstractProtocolsTransfers
                                             *p_transfers)
{
    return (uchar(p_transfers->receiveDataBuff[1]) * 256 +
            uchar(p_transfers->receiveDataBuff[2]));
}

uchar MasterProtocols::driverId(AbstractProtocolsTransfers
                                    *p_transfers)
{
    return uchar(p_transfers->receiveDataBuff[3]);
}

uchar MasterProtocols::packAnswerCommand(AbstractProtocolsTransfers
                                             *p_transfers)
{
    return uchar(p_transfers->receiveDataBuff[4]);
}

void MasterProtocols::inquiryRequest(uchar (*data)[RequestCommandBuffSize])
{
    (*data)[0] = uchar(RequestCommand::Inquiry);
}

uchar MasterProtocols::checksum(const uchar *data,
                                const unsigned short count)
{
    uchar sum = 0;
    uchar temp = 0;
    for (int i = 0; i < count; ++i)
    {
        temp = *(data + i);
        sum += temp;
    }
    return sum;
}

void MasterProtocols::focusRequest(
    uchar (*data)[RequestCommandBuffSize], const FocusRequest focus,
    const Zoom zoom)
{
    (*data)[0] = uchar(RequestCommand::Focus);
    (*data)[1] = uchar(focus);
    if (focus == FocusRequest::SetZoom)
    {
        (*data)[2] = zoom / 256;
        (*data)[3] = zoom % 256;
    }
}

void MasterProtocols::takePictureRequest(
    uchar (*data)[RequestCommandBuffSize],
    const TakePictureInfo *p_info)
{
    (*data)[0] = uchar(RequestCommand::TakePicture);
    (*data)[1] = uchar(p_info->resolution);
    (*data)[2] = uchar(p_info->isColor ? 0x00 : 0x01) + uchar(p_info->quality);
    (*data)[3] = uchar(p_info->imageFormat);
    (*data)[4] = uchar(p_info->exposure);
}

void MasterProtocols::getPicturePackRequest(uchar (*data)[RequestCommandBuffSize],
                                            const unsigned short numberOfPack, const PicturePackInfo *p_info)
{
    if (numberOfPack > p_info->sizeOfPack)
    {
        return;
    }
    (*data)[0] = uchar(RequestCommand::GetPicturePack);
    (*data)[1] = 0x01;
    (*data)[2] = numberOfPack / 256;
    (*data)[3] = numberOfPack % 256;
    (*data)[4] = p_info->sizeOfPack / 256;
    (*data)[5] = p_info->sizeOfPack % 256;
}

bool MasterProtocols::packCheck(AbstractProtocolsTransfers
                                    *p_transfers)
{
    uchar sum = MasterProtocols::checksum(p_transfers->receiveDataBuff + 1, AnswerCommandBuffSize - 3);
    uchar temp = packAnswerCommand(p_transfers);
    if (temp == uchar(AnswerCommand::GetPicturePack))
    {
        sum += MasterProtocols::checksum(
            p_transfers->p_PackBuff->getPointToPackBuff(),
            p_transfers->p_PackBuff->getSizeOfByte());
    }

    if (sum == p_transfers->receiveDataBuff[11] && p_transfers->receiveDataBuff[12] == 0xe7)
    {
        return true;
    }
    return false;
}

ErrorCode MasterProtocols::inquiryAnswer(AbstractProtocolsTransfers
                                             *p_transfers,
                                         DriverInfo *p_info)
{
    if (p_transfers->receiveDataBuff[6] != 0 ||
        p_transfers->receiveDataBuff[7] != 3)
    {
        return ErrorCode::DataError;
    }
    p_info->baudRate = BaudRate(p_transfers->receiveDataBuff[5]);
    p_info->hasFirmware = p_transfers->receiveDataBuff[9] == 0 ? true : false;
    p_info->isSpi = (p_transfers->receiveDataBuff[10] & 0x80) > 0 ? true : false;

    p_info->softwareAndHardware =
        p_transfers->receiveDataBuff[10] & 0x7f;

    return ErrorCode::NoneError;
}

ErrorCode MasterProtocols::takePictureAnswer(AbstractProtocolsTransfers
                                                 *p_transfers,
                                             PicturePackInfo *p_info)
{
    if (p_transfers->receiveDataBuff[5] == 0x00)
    {
        p_info->sizeOfByte = (p_transfers->receiveDataBuff[6] * 65536 +
                              p_transfers->receiveDataBuff[7] * 256 +
                              p_transfers->receiveDataBuff[8]);
        p_info->sizeOfPack = (p_transfers->receiveDataBuff[9] * 256 +
                              p_transfers->receiveDataBuff[10]);
        return ErrorCode::NoneError;
    }
    else
    {
        return ErrorCode(p_transfers->receiveDataBuff[5]);
    }
}

ErrorCode MasterProtocols::foucsAnswer(AbstractProtocolsTransfers
                                           *p_transfers,
                                       Zoom *p_zoom)
{
    if (p_transfers->receiveDataBuff[5] == 0x00)
    {
        *p_zoom = (p_transfers->receiveDataBuff[6] * 256 +
                   p_transfers->receiveDataBuff[7]);
        return ErrorCode::NoneError;
    }
    else
    {
        return ErrorCode(p_transfers->receiveDataBuff[5]);
    }
}

ErrorCode MasterProtocols::getPicturePackAnswer(AbstractProtocolsTransfers
                                                    *p_transfers,
                                                const PicturePackInfo *p_info)
{
    if (p_transfers->receiveDataBuff[5] == 0x01)
    {
        unsigned short numberOfPack = p_transfers->receiveDataBuff[6] * 256 +
                                      p_transfers->receiveDataBuff[7];
        if (numberOfPack < p_info->sizeOfPack)
        {
            p_transfers->p_PackBuff = new PackBuff(MaxSizeOfBuffByte,
                                                   numberOfPack);
            p_transfers->getLongByte(
                p_transfers->p_PackBuff->getPointToPackBuff(), MaxSizeOfBuffByte);
        }
        else
        {
            unsigned short sizeOfBity = p_info->sizeOfByte -
                                        MaxSizeOfBuffByte * (numberOfPack - 1);
            p_transfers->p_PackBuff = new PackBuff(sizeOfBity, numberOfPack);
            p_transfers->getLongByte(p_transfers->p_PackBuff->getPointToPackBuff(),
                                     sizeOfBity);
        }
        return ErrorCode::NoneError;
    }
    else
    {
        return ErrorCode::DataError;
    }
}
