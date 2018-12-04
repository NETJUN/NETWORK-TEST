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

void NetManager::dataSend(QByteArray data, int pMode) {
    switch (pMode) {
    case TCP_CLIENT_MODE:
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
    if(!tcpServer->listen(localHost, localPort)) {
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
