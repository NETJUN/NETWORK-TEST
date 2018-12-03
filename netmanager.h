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
    void setIpAndPort(QHostAddress pRemoteHost, quint16 pRemotePort, QHostAddress pLocalHost, quint16 pLocalPort);
    void TCPConnectToHost();
    void startListen();
    void dataSend(int pMode);
    void dataReceive(int pMode);
    void UDPStart();

signals:
    void TCPConnectedSignal();
    void messageReceived(QString msg);
    void updateDataStatus(QString status, QVariant inNum, QVariant outNum);
private slots:
    void TCPConnectionStatus();
    void UDPMessageReceived(QString);
    void UDPDataStatus(QString, QVariant, QVariant);

private:
    TCPServer *tcpServer;
    TCPClient *tcpClient;
    UDPClient *udpClient;

    QHostAddress remoteHost;
    quint16 remotePort;
    QHostAddress localHost;
    quint16 localPort;

    CONNECTION_MODE mode;
};

#endif // NETMANAGER_H
