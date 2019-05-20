#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

#include <thread>
#include <mutex>
#include <chrono>

struct conninfo{
    int id;
    char address[16];
    char hostname[64];
    char username[256];
    char deskname[25];
    char accesstype[5];
};

class TcpClient
{
public:
    TcpClient();
    ~TcpClient();
    int setConnect(const char * host, uint16_t port);
    int connectServer();
    void endConnection();

    ssize_t recvData(void *data, size_t szdata);
    ssize_t sendData(const void *data, size_t szdata);

private:
    struct sockaddr_in m_servaddr;
    struct hostent *m_host;
    int m_server;
    int m_sleep;
    std::mutex m_recvmtx;
    std::mutex m_sendmtx;
};

#endif // TCPCLIENT_H
