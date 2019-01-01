#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "abstractclient.h"

class QUdpSocket;

class UDPClient : public AbstractClient
{
    Q_OBJECT
public:
    explicit UDPClient(QObject *parent = 0);

public:
    virtual void connectToHost(const QHostAddress remoteIp, const quint16 port);
    virtual void sendData(const QByteArray &data);
    virtual void receiveData();

public:
   // void sendData(const QString string, const QString remoteIp, const int port);
    void udpStart(const QHostAddress localIp, const int listnerPort, const QHostAddress remoteIp, const int remotePort);
    void udpStop(const QString string, const QString remoteIp, const int port);
    void readyRead(QUdpSocket* socket);

signals:
    void valueChanged(QString newValue);
    void updateState(QString state, QVariant inNum, QVariant outNum);
public slots:
    void readyListnerRead();
    void readySendRead();
    void connection_error(QAbstractSocket::SocketError err);
private:
    QUdpSocket *udpSendSocket = nullptr;
    QUdpSocket *udpListnerSocket = nullptr;
    void udpListnerStart(const QHostAddress ip, const int port);
    void udpListnerStop();

    QHostAddress remoteIpAddress;
    quint16 remotePort;

    quint32 sendDataSize;
};

#endif // UDPCLIENT_H

