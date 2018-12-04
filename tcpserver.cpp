#include "tcpserver.h"
#include "tcpclientserver.h"

TCPServer::TCPServer(QObject* parent)
    :QTcpServer(parent) {

}

void TCPServer::incomingConnection(qintptr socketId){
    qDebug("%s", __func__);
    TCPClientServer *socket = new TCPClientServer(this);
    socket->setSocketDescriptor(socketId);
}
