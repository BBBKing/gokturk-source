#include "audio.h"

int sendMicrophone(TcpClient & tcpClient1)
{

    Record record1(44100,256,40*48);
    MicroPacket microPacket1{};
    microPacket1.packetType = PCKAUD;
    record1.openDevice("default");
    DivPacket divPacket1((char*)record1.getRecord(), record1.getBufferSize(), 49152);

    for(size_t i = 0; i < divPacket1.getIter(); ++i)
    {

        std::memcpy(microPacket1.packetBuffer, divPacket1.getPtr(i), 49152);
        tcpClient1.sendData(&microPacket1, sizeof(microPacket1));

    }



    record1.closeDevice();

    return 0;
}
