#ifndef SCREEN_H
#define SCREEN_H

extern "C"{
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
};


#include "compressor.h"
#include "packet.h"
#include "tcpclient.h"
#include "autobuffer.h"
#include "webcam.h"

int sendScreen(TcpClient & tcpClient1);
int sendCamera(TcpClient & tcpClient1, WebCam & webCam1);

#endif // SCREEN_H
