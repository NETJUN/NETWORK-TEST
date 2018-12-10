#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "abstractclient.h"

class QTcpSocket;

class TCPClient : public AbstractClient{
    Q_OBJECT
public:
    TCPClient(QObject *parent = 0);
    ~TCPClient();

public:
    virtual void connectToHost(const QHostAddress remoteIp, const quint16 port);
    virtual void sendData(const QByteArray &data);
    virtual void receiveData();

public:
    bool isConnected();
    void closeClient();

public slots:
    void dataArrived();
    void setConnectStatus();

signals:
    void connectedSignal();

private:
    QTcpSocket *tcpSocket;
    bool connected;
};

#endif // TCPCLIENT_H
