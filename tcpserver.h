#ifndef TCPSERVER_H
#define TCPSERCER_H
#include <QTcpServer>

class TCPServer : public QTcpServer{
    Q_OBJECT

public:
    TCPServer(QObject *parent = 0);

private:
    void incomingConnection(int socketId);
};

#endif // TCPSERCER_H
