#ifndef TCPClientSERVER_H
#define TCPClientSERVER_H
#include <QTcpSocket>

class TCPClientServer : public QTcpSocket{
    Q_OBJECT

public:
    TCPClientServer(QObject *parent = 0);

private slots:
    void readClient();

private:
    void generateRandomData(const QString &from, const QString &to, const QDate &date, const QTime &time);
    quint16 nextBlockSize;
};

#endif // CLIENTSERVER_H
