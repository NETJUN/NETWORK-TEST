#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "abstractclient.h"

class QTcpSocket;

class TCPClient : public AbstractClient{
public:
    TCPClient(QObject *parent = 0);
    ~TCPClient();

public:
    virtual void connectToHost(const QHostAddress remoteIp, const quint16 port);
    virtual void sendData(const QByteArray &data);
    virtual void receiveData(QByteArray &data);

public:
    bool isConnected();

private:
    void setConnectStatus();

private:
    QTcpSocket *tcpSocket;
    bool connected;
};

#endif // TCPCLIENT_H
