#include "tcpclient.h"
#include "DataOutputShow.h"
#include "ParameterCenter.h"
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
    for(int i = 0; i < 500; ++i) {
        tcpSocket->write(data);
    }
    emit DataOutputShow::getDataOutputInstance()->updateDataStatusSignal(QString(), QVariant(), data.size());
}

void TCPClient::closeClient() {
    tcpSocket->close();
}

void TCPClient::receiveData() {

}

void TCPClient::dataArrived() {
    // 收到下位机发来的数据请求，立即发送数据
    sendData(ParameterCenter::getParameterCenterInstance()->getInputData());
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
