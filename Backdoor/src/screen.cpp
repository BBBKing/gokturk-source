#include "screen.h"

int sendScreen(TcpClient & tcpClient1){

    Compressor compressor1;

    ScreenInfo screenInfo1{};

    Display * displayPtr = XOpenDisplay(0);
    Window rootWindow = DefaultRootWindow(displayPtr);

    XWindowAttributes wndAttr1;
    XGetWindowAttributes(displayPtr, rootWindow, &wndAttr1);

    screenInfo1.width = wndAttr1.width;
    screenInfo1.height= wndAttr1.height;
    screenInfo1.packetType = INFSCR;
    screenInfo1.bufferSize = screenInfo1.width * screenInfo1.height * 3;

    AutoBuffer autoBuffer1(screenInfo1.bufferSize);
    XImage * imagePtr = XGetImage(displayPtr, rootWindow, 0, 0 ,
                                static_cast<unsigned int>(screenInfo1.width),
                                    static_cast<unsigned int>( screenInfo1.height), AllPlanes, ZPixmap);

    int index =0;
    for (int x = 0; x < screenInfo1.width; ++x){

        for (int y = 0; y < screenInfo1.height ; ++y)
        {
            unsigned long pixel = XGetPixel(imagePtr,x,y);
            autoBuffer1[index]   = static_cast<char> ((pixel & imagePtr->red_mask) >> 16);
            autoBuffer1[index+1] = static_cast<char> ((pixel & imagePtr->green_mask) >> 8);
            autoBuffer1[index+2] = static_cast<char> ( pixel & imagePtr->blue_mask);
            index+=3;
        }
    }
    AutoBuffer autoBufferCompress1(screenInfo1.bufferSize);
    screenInfo1.compressSize = compressor1.compress((Bytef*)autoBuffer1.getPointer(), screenInfo1.bufferSize,
                                                    (Bytef*)autoBufferCompress1.getPointer(), screenInfo1.bufferSize);

    tcpClient1.sendData(&screenInfo1, sizeof(screenInfo1));

    DivPacket divPacket1(autoBufferCompress1.getPointer(), screenInfo1.compressSize, 50000);

    for(size_t i = 0; i < divPacket1.getIter(); ++i)
    {

        ScreenPacket screenPacket1{};
        screenPacket1.packetType = PCKSCR;
        std::memcpy(screenPacket1.packetBuffer, divPacket1.getPtr(i), divPacket1.getSize(i));

        screenPacket1.packetSize = divPacket1.getSize(i);
        int senderSize = 50000 - divPacket1.getSize(i);

        tcpClient1.sendData(&screenPacket1, sizeof(screenPacket1) - senderSize);

    }

    XDestroyImage(imagePtr);
    XCloseDisplay(displayPtr);

    return 0;

}

int sendCamera(TcpClient & tcpClient1, WebCam & webCam1)
{
    webCam1.getCapture();

    CameraInfo cameraInfo1{};
    cameraInfo1.packetType = INFCAM;
    cameraInfo1.bufferSize = webCam1.getBufferSize();

    tcpClient1.sendData(&cameraInfo1, sizeof(cameraInfo1));
    DivPacket divPacket1((char*)webCam1.getBuffer(), webCam1.getBufferSize(), 50000);

    for(size_t i = 0; i < divPacket1.getIter(); ++i)
    {

        CameraPacket cameraPacket1{};
        cameraPacket1.packetType = PCKCAM;
        std::memcpy(cameraPacket1.packetBuffer, divPacket1.getPtr(i), divPacket1.getSize(i));

        cameraPacket1.packetSize = divPacket1.getSize(i);
        int senderSize = 50000 - divPacket1.getSize(i);

        tcpClient1.sendData(&cameraPacket1, sizeof(cameraPacket1) - senderSize);

    }
    return 0;
}


