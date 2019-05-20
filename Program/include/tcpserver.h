#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include <thread>
#include <vector>
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

class TcpServer
{

public:
    TcpServer();
    ~TcpServer();

    int setListen(uint16_t port, int maxcon);
    int acceptConnections();

    void endConnection();
    const std::vector<conninfo>& getConnections();

    ssize_t sendData(int id, const void *data, size_t szdata);
    ssize_t recvData(int id, void *data, size_t szdata);

private:
    struct sockaddr_in m_servaddr;
    struct sockaddr_in m_cliaddr;
    int m_server;
    int m_client;
    int m_sleep;
    std::mutex m_recvmtx;
    std::mutex m_sendmtx;
    std::vector<conninfo> m_connections;
};

#endif // TCPSERVER_H
