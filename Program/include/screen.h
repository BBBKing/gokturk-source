#ifndef SCREEN_H
#define SCREEN_H


extern "C"{
#include <X11/Xlib.h>
#include <X11/X.h>
}

#include "../include/compressor.h"
#include "../include/tcpserver.h"
#include "../include/packet.h"
#include "../include/autobuffer.h"

#include <jpeglib.h>
#include <jerror.h>

#define cimg_plugin "plugins/jpeg_buffer.h"
#include <CImg.h>
using namespace cimg_library;



class DesktopScreen
{
    public:
        DesktopScreen();
        ~DesktopScreen();
        void onShow();
        int renderDisplay(int compressSize, int width, int height,const char* imageBuffer);
        bool is_Closed();

    private:
         CImgDisplay m_Display;


};



class CamScreen
{
    public:
        CamScreen();
        ~CamScreen();
        void onShow();
        int renderDisplay(const char* imageBuffer, size_t bufferSize);
        bool is_Closed();

    private:
         CImgDisplay m_Display;
};

#endif // SCREEN_H
