#include "ParameterCenter.h"

ParameterCenter* ParameterCenter::obj = NULL;

ParameterCenter::ParameterCenter() {

}

/**
 * @brief ParameterCenter::setInputData
 * @param pData
 * @details 手动输入的数据
 */
void ParameterCenter::setInputData(const QByteArray &pData) {
    inputData = pData;
}

const QByteArray& ParameterCenter::getInputData() {
    return inputData;
}

/**
 * @brief ParameterCenter::setLocalPort
 * @param port
 * @details 本地端口
 */
void ParameterCenter::setLocalPort(quint16 port) {
    if(port < 0) {
        qDebug("parameter error!");
        return;
    }
    localPort = port;
}

quint16 ParameterCenter::getLocalPort() {
    return localPort;
}

/**
 * @brief ParameterCenter::setRemotePort
 * @param port
 * @details 远端端口
 */
void ParameterCenter::setRemotePort(quint16 port) {
    if(port < 0) {
        qDebug("parameter error!");
        return;
    }
    remotePort = port;
}

quint16 ParameterCenter::getRemotePort() {
    return remotePort;
}

/**
 * @brief ParameterCenter::setLocalIP
 * @param addr
 * @details 本地IP
 */
void ParameterCenter::setLocalIP(const QHostAddress &addr) {
    if(addr.isNull()) {
        qDebug("parameter error!");
        return;
    }
    localIP = addr;
}

QHostAddress ParameterCenter::getLocalIP() {
    return localIP;
}

/**
 * @brief ParameterCenter::setRemoteIP
 * @param addr
 * @details 远端IP
 */
void ParameterCenter::setRemoteIP(const QHostAddress &addr) {
    if(addr.isNull()) {
        qDebug("parameter error!");
        return;
    }
    remoteIP = addr;
}

QHostAddress ParameterCenter::getRemoteIP() {
    return remoteIP;
}
