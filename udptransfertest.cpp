#include "udptransfertest.h"

UdpTransferTest::UdpTransferTest(QFile* pFile)
    :file(pFile), udpRecvFlag(false){

}

void UdpTransferTest::setUdpRecvMsg(QString str) {
    udpRecvFlag = true;
    udpData = str;
}

void run() {
    QByteArray data;
    if(file == nullptr) {
        qDebug() << "file is empty!!!";
        return;
    }
    data = file->read(10);
    while(data.size() != 0) {
        netManager->dataSend(data, mode);
        while(!udpDataFlag) {
            msleep(10);
        }
        udpDataFlag = false;
        if(data != udpRecvData) {
            qDebug() << "file check failed!";
            return;
        }
        data = file->read(100);
    }
    file->close();
    qDebug() << "file check success!";
}
