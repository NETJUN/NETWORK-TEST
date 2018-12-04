#include "tcpclientserver.h"
#include <QDate>
#include <QDataStream>
#include <QTime>
#include <QHostAddress>

TCPClientServer::TCPClientServer(QObject *parent)
    :QTcpSocket(parent){
    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));

    nextBlockSize = 0;
}

void TCPClientServer::readClient(){
   /*
    QDataStream in(this);

    if(nextBlockSize == 0){
        if(bytesAvailable() < sizeof(quint16)){
            return;
        }
        in >> nextBlockSize;
    }
    if(bytesAvailable() < nextBlockSize){
        return;
    }
    quint8 requestType;
    QString from;
    QString to;
    QDate date;
    QTime time;
    quint8 flag;

    in >> requestType;
    if(requestType == 'S'){
        in >> from >> to >> date >> time >> flag;
    }

    std::srand(from.length() * 3600 + to.length() * 60 + time.hour());
    int num = std::rand() % 8;
    for(int i = 0; i < num; ++i){
        generateRandomData(from, to, date, time);
    }

    QDataStream out(this);
    out << quint16(0xFFFF);
    */
    qDebug() << "received data from:" << peerAddress().toString() << QString("%1").arg(peerPort());
}

void TCPClientServer::generateRandomData(const QString &from, const QString &to, const QDate &date, const QTime &time){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    quint16 duration = std::rand() % 200;
    out << quint16(0) << date << time << duration << quint8(1) << QString("InterCity");
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    write(block);
}
