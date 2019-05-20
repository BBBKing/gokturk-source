#include "tcpclient.h"


int TcpClient::setConnect(const char * host, uint16_t port){

    m_host   = gethostbyname(host);
    m_server = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    if (setsockopt(m_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
       //
    }

    if(m_server < 0){
        return -1;
    }
    std::memcpy(m_host->h_addr, (char *)&m_servaddr.sin_addr.s_addr, (size_t)m_host->h_length);
    m_servaddr.sin_family = AF_INET;
    m_servaddr.sin_port = htons(port);

    return 0;
}

int TcpClient::connectServer(){

    return connect(m_server,(struct sockaddr *)&m_servaddr,sizeof(m_servaddr));

}

void TcpClient::endConnection(){

    shutdown(m_server, SHUT_RDWR);
    close(m_server);
}
ssize_t TcpClient::sendData(const void *data, size_t szdata)
{
    std::lock_guard<std::mutex> sendguard(m_sendmtx);
    const auto *pdata = (const unsigned char*) data;
    ssize_t sendbytes=0;

    while (szdata > 0)
    {
        sendbytes = send(m_server, pdata, szdata, 0);
        if (sendbytes < 0){
            return sendbytes;
        }


        pdata += sendbytes;
        szdata -= sendbytes;
    }

    return sendbytes;
}


ssize_t TcpClient::recvData(void *data, size_t szdata)
{
    std::lock_guard<std::mutex> recvguard(m_recvmtx);
    auto *pdata = (unsigned char*) data;
    ssize_t recvbytes =0;

    while (szdata > 0)
    {
        recvbytes = recv(m_server, pdata, szdata, 0);
        if (recvbytes <= 0){
            return recvbytes;
        }


        pdata    += recvbytes;
        szdata   -= recvbytes;
    }

    return recvbytes;
}

TcpClient::TcpClient()
{

}

TcpClient::~TcpClient()
{
    close(m_server);

}
