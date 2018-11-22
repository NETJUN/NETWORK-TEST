#include "tcpserver.h"
#include "tcpclientserver.h"

TCPServer::TCPServer(QObject *parent)
    :QTcpServer(parent){

}

void TCPServer::incomingConnection(int socketId){
    TCPClientServer *socket = new TCPClientServer(this);
    socket->setSocketDescriptor(socketId);
}
