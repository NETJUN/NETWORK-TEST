#include "netmanager.h"

NetManager::NetManager(QObject *parent) {
    tcpClient = new TCPClient(this);
    udpClient = new UDPClient(this);

    connect(tcpClient, SIGNAL(connectedSignal()), this, SLOT(TCPConnectionStatus()));
    connect(udpClient, SIGNAL(valueChanged(QString)), this, SLOT(UDPMessageReceivedSignal(QString)));
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

void NetManager::dataSend(QByteArray data, CONNECTION_MODE pMode) {
    switch (pMode) {
    case TCP_CLIENT_MODE:
        tcpClient->connectToHost(remoteHost, remotePort);
        tcpClient->sendData(data);
        break;
    case TCP_SERVER_MODE:
        break;
    case UDP_MODE:
        udpClient->connectToHost(remoteHost, remotePort);
        udpClient->sendData(data);
    default:
        break;
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
    if(!tcpServer->listen(QHostAddress("192.168.1.100"), localPort)) {
        delete tcpServer;
        tcpServer = NULL;
        qDebug("Failed to bind to port");
        return;
    }
    qDebug() << "listenning ip:" << localHost.toString() << " Port:" + QString("%1").arg(localPort);
}

void NetManager::UDPStart() {
    udpClient->udpStart(localHost, localPort, remoteHost, remotePort);
}

void NetManager::UDPStop() {
    udpClient->udpStop(NULL, NULL, NULL);
}

void NetManager::TCPConnectionStatus() {
    emit TCPConnectedSignal();
}

void NetManager::UDPMessageReceivedSignal(QString msg) {
    emit messageReceivedSignal(msg);
}

void NetManager::UDPDataStatus(QString status, QVariant inNum, QVariant outNum) {
    emit updateDataStatusSignal(status, inNum, outNum);
}

/**
 * @brief ¶Ï¿ªÍøÂç
 * @param CONNECTION_MODE pMode
 * @author yxj
 */
void NetManager::disConnectNet(CONNECTION_MODE pMode) {
    switch (pMode) {
    case TCP_CLIENT_MODE:
        tcpClient->closeClient();
        break;
    case TCP_SERVER_MODE:
        delete tcpServer;
        break;
    case UDP_MODE:
        UDPStop();
        break;
    default:
        break;
    }
}
