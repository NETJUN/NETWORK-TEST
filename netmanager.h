#ifndef NETMANAGER_H
#define NETMANAGER_H

#include "tcpclientserver.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "udpclient.h"
#include "BaseDef.h"
#include <QObject>


class NetManager : public QObject {
    Q_OBJECT
public:
    NetManager(QObject *parent = 0);
    ~NetManager();

public:
    void setIpAndPort(QHostAddress host, quint16 port);
    void startListen();
    void dataSend(int pMode);
    void dataReceive(int pMode);

private:
    TCPServer *tcpServer;
    TCPClient *tcpClient;
    UDPClient *udpClient;

    QHostAddress remoteIpAddress;
    quint16 remotePort;

    CONNECTION_MODE mode;
};

#endif // NETMANAGER_H
