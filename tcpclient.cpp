#include "tcpclient.h"
#include <QTcpSocket>

TCPClient::TCPClient(QObject *parent)
    :tcpSocket(new QTcpSocket(parent)){

    connected = false;
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(setConnectStatus()));
}

TCPClient::~TCPClient(){
    delete tcpSocket;
}

void TCPClient::connectToHost(const QHostAddress remoteIp, const quint16 port){
    tcpSocket->connectToHost(remoteIp, port);
}

void TCPClient::sendData(const QByteArray &data){
    tcpSocket->write(data);
}

void TCPClient::receiveData(QByteArray &data){

}

void TCPClient::setConnectStatus(){
    connected = true;
}

bool TCPClient::isConnected(){
    return connected;
}
