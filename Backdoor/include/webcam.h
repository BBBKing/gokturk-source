#ifndef WEBCAM_H
#define WEBCAM_H

#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <cstring>


class WebCam
{

    v4l2_buffer     m_CapBuffer;
    v4l2_requestbuffers m_RequestBuffers;
    v4l2_capability m_Capability;
    v4l2_cropcap    m_CropCap;
    v4l2_fmtdesc    m_FmtDesc;
    v4l2_format     m_Format;
    timeval         m_TimeVal;
    v4l2_buffer     m_MapBuffer;
    fd_set    m_Fdset;

    int       m_Device;
    uint8_t * m_CaptureBuffer;

public:
    WebCam();
    ~WebCam();

    int  errIoctl(int device, unsigned long request, void *argument);
    int  openDevice(const char * deviceName );
    int  closeDevice();

    int  setCropCapability(__u32 type);
    int  setFormatDescription(__u32 type);
    int  setFormatType(__u32 width,__u32 height ,__u32 type, __u32 pixelFormat, __u32 field);
    int  setRequestBuffer(__u32 reqType, __u32 reqMemory, __u32 reqCount);
    int  setMemoryMap(__u32 bufType, __u32 bufMemory, __u32 bufIndex);
    int  setCapture(int64_t timeOut, __u32 bufType, __u32 bufMemory, __u32 bufIndex );
    int  getDevice();
    int  getCapture();
    const char * getBuffer();
    int getBufferSize();
};

#endif // WEBCAM_H
