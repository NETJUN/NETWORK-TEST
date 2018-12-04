#ifndef TCPSERVER_H
#define TCPSERCER_H
#include <QTcpServer>


class TCPServer : public QTcpServer{
    Q_OBJECT

public:
    TCPServer(QObject* parent);

private:
    void incomingConnection(qintptr socketId);
};

#endif // TCPSERCER_H
