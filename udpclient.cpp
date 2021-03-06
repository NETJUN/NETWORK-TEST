﻿#include "udpclient.h"
#include "ParameterCenter.h"
#include <QMessageBox>
#include <QUdpSocket>
#include <iostream>

using namespace  std;


UDPClient::UDPClient(QObject *parent)
    :sendDataSize(0){
    qDebug("%s", __func__);
}

/**
 *连接主机
 * @brief UDPClient connect
 * @param remoteIp 目标IP
 * @param port 目标端口号
 */
void UDPClient::connectToHost(const QHostAddress remoteIp, const quint16 port){
    remoteIpAddress = remoteIp;
    remotePort = port;
}

/**
 * @brief MyUDP::sendData
 * @param string   发送内容
 * @param remoteIp 目标IP地址
 * @param port     目标端口号
 */
void UDPClient::sendData(const QByteArray &data)
{
    //qDebug("%s", __func__);
    if(udpSendSocket == nullptr)
        return;
    QByteArray bytearray;
    for(int i = 0; i < sendDataSize; ++i) {
        if(i & 0x01)
            bytearray.push_back(QByteArray("1111"));
        else
            bytearray.push_back(QByteArray("2222"));
    }
    qint64 sendSize = udpSendSocket->writeDatagram(bytearray, bytearray.size(), remoteIpAddress, remotePort);
    if(sendSize != bytearray.size()) {
        qDebug("输出有问题");
    }
    //emit updateState(QString(), QVariant(QVariant::Int), sendDataSize * data.size());
}

/**
 * @brief UDPClient receiceData
 */
void UDPClient::receiveData(){

}

/**
 * 处理网络连接错误
 * @brief MyUDP::connection_error
 * @param err
 */
void UDPClient::connection_error(QAbstractSocket::SocketError err)
{
    qDebug("%s", __func__);
    switch(err){
    case 0:QMessageBox::critical(0,"connection error","The connection was refused by the peer (or timed out).",QMessageBox::Ok);
           break;
    case 2:QMessageBox::critical(0,"connection error","The host address was not found.",QMessageBox::Ok);
           break;
    case QAbstractSocket::NetworkError:QMessageBox::critical(0,"connection error","An error occurred with the network .",QMessageBox::Ok);
           break;

    case QAbstractSocket::RemoteHostClosedError:
           QMessageBox::critical(0,"connection error","disconnect .",QMessageBox::Ok);
           break;
    default:QMessageBox::critical(0,"connection error","undefine error.",QMessageBox::Ok);
        qDebug()<<"error is ......."<<err;
        break;
    }
}


/**
 * @brief UDPClient::udpListnerStart
 * @param ip    监听IP地址
 * @param port  监听端口号
 */
void UDPClient::udpListnerStart(const QHostAddress ip, const int port)
{
    qDebug("%s", __func__);
    udpListnerSocket = new QUdpSocket(this);
    if(!udpListnerSocket->bind(ip, port)) {
        qWarning("NULL");
    }
    connect(udpListnerSocket, SIGNAL(readyRead()), this, SLOT(readyListnerRead()));
    connect(udpListnerSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connection_error(QAbstractSocket::SocketError)));
}

/**
 * @brief UDPClient::udpListnerStop
 * 停止监听
 */
void UDPClient::udpListnerStop()
{
    qDebug("%s", __func__);
    if(udpListnerSocket != nullptr) {
        udpListnerSocket->close();
        udpListnerSocket = nullptr;
    }
}

/**
 * @brief UDPClient::udpStart
 * @param localIp
 * @param listnerPort
 * @param remoteIp
 * @param remotePort
 * 启动UDP服务
 */
void UDPClient::udpStart(const QHostAddress localIp, const int listnerPort, const QHostAddress remoteIp, const int remotePort)
{
    qDebug("%s", __func__);
    // 开启发送
    if(udpSendSocket == nullptr) {
        udpSendSocket = new QUdpSocket(this);
        connect(udpSendSocket, SIGNAL(readyRead()), this, SLOT(readySendRead()));
        connect(udpSendSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connection_error(QAbstractSocket::SocketError)));
    }
    this->remoteIpAddress = remoteIp;
    this->remotePort = remotePort;
    // 开启接收Socket
    udpListnerStart(localIp, listnerPort);
}

/**
 * @brief UDPClient::udpStop
 * @param string
 * @param remoteIp
 * @param port
 * 停止UDP服务
 */
void UDPClient::udpStop(const QString string, const QString remoteIp, const int port)
{
    qDebug("%s", __func__);
    // 关闭发送Socket
    if(udpSendSocket != nullptr) {
        udpSendSocket->close();
        udpSendSocket = nullptr;
    }

    // 关闭Listner
    udpListnerStop();
}

/**
 * @brief UDPClient::readyListnerRead
 * 监听读到数据
 */
void UDPClient::readyListnerRead()
{
   // qDebug("%s", __func__);
    readyRead(udpListnerSocket);
}

/**
 * @brief UDPClient::readyRead
 * @param socket
 * 读取数据
 */
void UDPClient::readyRead(QUdpSocket* socket)
{
    //qDebug("%s", __func__);
    QByteArray Buffer;
    Buffer.resize(socket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
    QString str("0x");

    for(int i = Buffer.size(); i >= 0; --i) {
        int a = (int)Buffer[i];
        str += QString::number(a, 10);
    }

    bool ok;
    sendDataSize = str.toUInt(&ok, 16);


    //qDebug() << "Message from:" << sender.toString();
    //qDebug() << "Message port:" << senderPort;
    qDebug() << "Message: " << sendDataSize;

    //emit valueChanged(Buffer);
    //emit updateState(QString(), Buffer.size(), QVariant(QVariant::Int));

    sendData(ParameterCenter::getParameterCenterInstance()->getInputData());
}

/**
 * @brief UDPClient::readySendRead
 * 发送得到回传数据
 */
void UDPClient::readySendRead()
{
    qDebug("%s", __func__);
    readyRead(udpSendSocket);
}
