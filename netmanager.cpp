#include "netmanager.h"

NetManager::NetManager(QObject *parent) {
    tcpClient = new TCPClient(this);
    udpClient = new UDPClient(this);

    connect(tcpClient, SIGNAL(connectedSignal()), this, SLOT(TCPConnectionStatus()));
    connect(udpClient, SIGNAL(valueChanged(QString)), this, SLOT(UDPMessageReceived(QString)));
    connect(udpClient, SIGNAL(updateState(QString,QVariant,QVariant)), this, SLOT(UDPDataStatus(QString, QVariant, QVariant)));
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

void NetManager::setIpAndPort(QHostAddress pRemoteHost, quint16 pRemotePort, QHostAddress pLocalHost, quint16 pLocalPort) {
    remoteHost = pRemoteHost;
    remotePort = pRemotePort;
    localHost = pLocalHost;
    localPort = pLocalPort;
}

void NetManager::TCPConnectToHost() {
    tcpClient->connectToHost(remoteHost, remotePort);
}

void NetManager::startListen() {
    tcpServer = new TCPServer(this);
    if(tcpServer->listen(QHostAddress::Any, 8088)) {
        qDebug("Failed to bind to port");
        return;
    }
}

void NetManager::UDPStart() {
    udpClient->udpStart(localHost, localPort, remoteHost, remotePort);
}

void NetManager::TCPConnectionStatus() {
    emit TCPConnectedSignal();
}

void NetManager::UDPMessageReceived(QString msg) {
    emit messageReceived(msg);
}

void NetManager::UDPDataStatus(QString status, QVariant inNum, QVariant outNum) {
    emit updateDataStatus(status, inNum, outNum);
}
