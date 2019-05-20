#include "tcpserver.h"

int TcpServer::setListen(uint16_t port, int maxcon){

        m_servaddr.sin_family = AF_INET;
        m_servaddr.sin_addr.s_addr = INADDR_ANY;
        m_servaddr.sin_port = htons(port);

        m_server = socket(AF_INET, SOCK_STREAM, 0);
        if(m_server < 0){
            return -1;
        }

    int opt = 1;
    if (setsockopt(m_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {

    }

        if(bind(m_server, (struct sockaddr *) &m_servaddr,sizeof(m_servaddr))< 0){
            return -1;
        }
        if(listen(m_server, maxcon) < 0){
            return -1;
        }
return 0;

}


ssize_t TcpServer::sendData(int id, const void *data, size_t szdata)
{
    //std::lock_guard<std::mutex> sendguard(m_sendmtx);
    const auto *pdata = (const unsigned char*) data;
    ssize_t sendbytes=0;

    while (szdata > 0)
    {
        sendbytes = send(m_connections[id].id, pdata, szdata, 0);
        if (sendbytes < 0){
            return sendbytes;
        }


        pdata += sendbytes;
        szdata -= sendbytes;
    }

    return sendbytes;
}


ssize_t TcpServer::recvData(int id, void *data, size_t szdata)
{
   // std::lock_guard<std::mutex> recvguard(m_recvmtx);
    auto *pdata = (unsigned char*) data;
    ssize_t recvbytes =0;

    while (szdata > 0)
    {
        recvbytes = recv(m_connections[id].id, pdata, szdata, 0);
        if (recvbytes <= 0){
            return recvbytes;
        }
        pdata    += recvbytes;
        szdata   -= recvbytes;
    }

    return recvbytes;
}


void TcpServer::endConnection(){

    for(auto i: m_connections){
        shutdown(i.id, SHUT_RDWR);
        close(i.id);
    }
    shutdown(m_server, SHUT_RDWR);
    close(m_server);
    m_server = -1;
    m_connections.clear();

}

const std::vector<conninfo>& TcpServer::getConnections(){


    return m_connections;
}

int TcpServer::acceptConnections() {
        conninfo sysinfo;
        int clilen = sizeof(m_cliaddr);

        m_client = accept(m_server, (struct sockaddr *) &m_cliaddr, (socklen_t*)&clilen);
        if(m_client >= 0 && m_server != -1){
                sysinfo.id = m_client;
                m_connections.push_back(sysinfo);
               if(recvData(m_connections.size()-1,&sysinfo,sizeof(sysinfo)) <= 0){
                        m_connections.erase(m_connections.begin() + m_connections.size()-1);
                     return -1;
                }
                m_connections.erase(m_connections.begin() + m_connections.size()-1);
                sysinfo.id = m_client;
                strncpy(sysinfo.address,inet_ntoa(m_cliaddr.sin_addr),16);
                m_connections.push_back(sysinfo);

        }
    return m_client;
}




TcpServer::TcpServer()
{
    //ctor
}

TcpServer::~TcpServer()
{

    close(m_client);
    //dtor
}
