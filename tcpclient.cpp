#include "tcpclient.h"
#include "DataOutputShow.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>

TCPClient::TCPClient(QObject *parent)
    :tcpSocket(new QTcpSocket(parent)) {

    connected = false;
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(setConnectStatus()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataArrived()));
}

TCPClient::~TCPClient() {
    delete tcpSocket;
}

void TCPClient::connectToHost(const QHostAddress remoteIp, const quint16 port) {
    tcpSocket->connectToHost(remoteIp, port);
}

void TCPClient::sendData(const QByteArray &data) {
    tcpSocket->write(data);
    emit DataOutputShow::getDataOutputInstance()->updateDataStatusSignal(QString(), QVariant(), data.size());
}

void TCPClient::receiveData() {

}

void TCPClient::dataArrived() {
    QDataStream in(tcpSocket);
    qint8 word;
    QByteArray data;
    data.push_back("Data From Server:");
    while(!in.atEnd()) {
         in >> word;
         data.push_back(word);
    }
    emit DataOutputShow::getDataOutputInstance()->messageReceivedSignal(data);
    emit DataOutputShow::getDataOutputInstance()->updateDataStatusSignal(QString(), data.size(), QVariant());
}

void TCPClient::setConnectStatus() {
    connected = true;
    emit connectedSignal();
}

bool TCPClient::isConnected() {
    return connected;
}
