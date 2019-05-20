#ifndef AUDIO_H
#define AUDIO_H
#include <cstring>
#include "microphone.h"
#include "packet.h"
#include "tcpclient.h"

int sendMicrophone(TcpClient & tcpClient1);

#endif // AUDIO_H
