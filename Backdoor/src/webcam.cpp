#include "webcam.h"


WebCam::WebCam()
 :  m_CapBuffer{}, m_RequestBuffers{}, m_Capability{},
    m_CropCap{}, m_FmtDesc{}, m_Format{},
    m_TimeVal{}, m_Device(-1), m_CaptureBuffer{nullptr}

{

}

WebCam::~WebCam() {

    if(m_Device != -1){

        closeDevice();
    }

}

int WebCam::errIoctl(int device, unsigned long request, void *argument) {

    int returnVal;

    do {
        returnVal = ioctl (device, request, argument);
    }

    while ( returnVal == -1 && EINTR == errno);

    return returnVal;

}


int WebCam::openDevice(const char *deviceName) {

    m_Device = open(deviceName, O_RDWR);
    if (m_Device == -1)
    {
        return -1;
    }

    if (errIoctl(m_Device, VIDIOC_QUERYCAP, &m_Capability) == -1)
    {
        return 1;
    }

    return 0;
}

int WebCam::setCropCapability(__u32 type) {

    m_CropCap.type = type;

        if (errIoctl (m_Device, VIDIOC_CROPCAP, &m_CropCap) == -1)
        {
                return -1;
        }
    return 0;
}

int WebCam::setFormatDescription(__u32 type) {

    int grbg_10 = 0;
    m_FmtDesc.type = type;
    while ( errIoctl(m_Device, VIDIOC_ENUM_FMT, &m_FmtDesc)==0)
    {
        if (m_FmtDesc.pixelformat == V4L2_PIX_FMT_RGB24) {
            grbg_10 = 1;
        }
        m_FmtDesc.index++;
    }
    if (!grbg_10)
    {
        return -1;
    }

    return 0;
}


int WebCam::setFormatType(__u32 width,__u32 height ,__u32 type, __u32 pixelFormat, __u32 field) {

    m_Format.fmt.pix.width = width;
    m_Format.fmt.pix.height = height;
    m_Format.type = type;
    m_Format.fmt.pix.pixelformat = pixelFormat;
    m_Format.fmt.pix.field = field;

    if (errIoctl(m_Device, VIDIOC_S_FMT, &m_Format) == -1)
    {
        return -1;
    }

    return 0;
}
int WebCam::setRequestBuffer(__u32 reqType, __u32 reqMemory, __u32 reqCount) {

    m_RequestBuffers.type = reqType;
    m_RequestBuffers.count = reqCount;
    m_RequestBuffers.memory = reqMemory;

    if (errIoctl(m_Device, VIDIOC_REQBUFS, &m_RequestBuffers) == -1)
    {
        return -1;
    }
    return 0;
}

int WebCam::setMemoryMap( __u32 bufType, __u32 bufMemory, __u32 bufIndex)
{
    m_MapBuffer.memory = bufMemory;
    m_MapBuffer.type   = bufType;
    m_MapBuffer.index  = bufIndex;

    if(errIoctl(m_Device, VIDIOC_QUERYBUF, &m_MapBuffer) == -1)
    {
        return -1;
    }

    m_CaptureBuffer = static_cast<uint8_t *>(mmap(nullptr, m_MapBuffer.length, PROT_WRITE | PROT_READ
                                       , MAP_SHARED, m_Device, m_MapBuffer.m.offset));
    if(m_CaptureBuffer == MAP_FAILED){
        return -1;
    }

    return 0;
}

int WebCam::setCapture(int64_t timeOut, __u32 bufType, __u32 bufMemory, __u32 bufIndex) {
    m_CapBuffer.type = bufType;
    m_CapBuffer.memory = bufMemory;
    m_CapBuffer.index = bufIndex;
    m_TimeVal.tv_sec = timeOut;
    return 0;
}


int WebCam::getCapture() {

    if(errIoctl(m_Device, VIDIOC_QBUF, &m_CapBuffer) == -1)
    {
        return -1;
    }

    if(errIoctl(m_Device, VIDIOC_STREAMON, &m_CapBuffer.type) == -1)
    {
        return -1;
    }

    FD_ZERO(&m_Fdset);
    FD_SET(m_Device, &m_Fdset);

    if(select(m_Device + 1, &m_Fdset, nullptr, nullptr, &m_TimeVal) == -1)
    {
        return -1;
    }

    if(errIoctl(m_Device, VIDIOC_DQBUF, &m_CapBuffer) == -1)
    {
        return -1;
    }

    return 0;
}

const char * WebCam::getBuffer()
{
    return (char*)m_CaptureBuffer;
}

int WebCam::getBufferSize()
{
    return m_CapBuffer.length;
}


int WebCam::closeDevice()
{
    if(errIoctl(m_Device, VIDIOC_STREAMOFF, &m_CapBuffer.type) == -1)
    {
        return -1;
    }

    if(munmap(m_CaptureBuffer, m_MapBuffer.length) == -1)
    {
        return -1;
    }

    if( close(m_Device) == -1)
    {
        return -1;
    }
    m_Device = -1;

    return 0;
}

int WebCam::getDevice()
{
    return m_Device;
}

