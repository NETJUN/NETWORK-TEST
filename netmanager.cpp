#include "netmanager.h"

NetManager::NetManager(QObject *parent) {

}

NetManager::~NetManager() {
    if(tcpServer != nullptr) {
        delete tcpServer;
    }
    if(tcpClient != nullptr) {
        delete tcpClient;
    }
    if(udpClient != nullptr) {
        delete udpClient;
    }
}

void NetManager::setIpAndPort(QHostAddress host, quint16 port) {
    remoteIpAddress = host;
    remotePort = port;
}
