#ifndef ABSTRACTCLIENT_H
#define ABSTRACTCLIENT_H

#include <QObject>
#include <QHostAddress>

class AbstractClient : public QObject{
    Q_OBJECT
public:
    AbstractClient(QObject *parent = 0){

    }

    virtual ~AbstractClient(){

    }

    virtual void connectToHost(const QHostAddress remoteIp, const quint16 port) = 0;
    virtual void sendData(const QByteArray &data) = 0;
    virtual void receiveData(QByteArray &data) = 0;
};

#endif // ABSTRACTCLIENT_H
