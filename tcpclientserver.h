#ifndef TCPClientSERVER_H
#define TCPClientSERVER_H
#include <QTcpSocket>

class TCPClientServer : public QTcpSocket{
    Q_OBJECT

public:
    TCPClientServer(QObject *parent);

signals:
    void valueChanged(QString newValue);
    void updateState(QString state, QVariant inNum, QVariant outNum);

private slots:
    void readClient();

private:
    void setSignalAndSlotConnect();

private:
    void replyMessageByServer(QByteArray &data);
    quint16 nextBlockSize;

};

#endif // CLIENTSERVER_H
