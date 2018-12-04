#include "tcpclientserver.h"
#include "DataOutputShow.h"
#include <QDate>
#include <QDataStream>
#include <QTime>
#include <QHostAddress>

TCPClientServer::TCPClientServer(QObject *parent)
    :QTcpSocket(parent){
    setSignalAndSlotConnect();
    nextBlockSize = 0;
}

void TCPClientServer::setSignalAndSlotConnect() {
    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void TCPClientServer::readClient() {
   QDataStream in(this);
   qint8 word;
   QByteArray data;
   data.push_back("Data From IP:" + peerAddress().toString().toLocal8Bit());
   data.push_back("Port:");
   data.push_back(QString("%1").arg(peerPort()).toLocal8Bit());
   while(!in.atEnd()) {
        in >> word;
        data.push_back(word);
   }
   emit DataOutputShow::getDataOutputInstance()->messageReceivedSignal(data);
   emit DataOutputShow::getDataOutputInstance()->updateDataStatusSignal(QString(), data.size(), QVariant());
}

void TCPClientServer::replyMessageByServer(QByteArray &data) {

}
