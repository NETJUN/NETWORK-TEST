#ifndef UDPTRANSFERTEST_H
#define UDPTRANSFERTEST_H

#include <QThread>
#include <QFile>

class UdpTransferTest : QThread {
public:
    UdpTransferTest(QFile *file);

public:
    void setUdpRecvMsg(QString str);

protected:
    run();

private:
    File* file;
    QString udpData;
    bool udpRecvFlag;
};

#endif // UDPTRANSFERTEST_H
