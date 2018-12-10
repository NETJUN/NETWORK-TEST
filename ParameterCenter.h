#ifndef PARAMETERCENTER_H
#define PARAMETERCENTER_H

#include <QObject>
#include <QHostAddress>

/**
 * @brief The ParameterCenter class
 * @author yxj
 * @date 2018/12/10
 * @details 该类为单例模式，作为软件的参数中心，提供参数获取的接口给软件的各个模块
 */

class ParameterCenter : public QObject {
    Q_OBJECT
private:
    ParameterCenter();
    static ParameterCenter* obj;

public:
    static ParameterCenter* getParameterCenterInstance() {
        if(obj == NULL) {
            obj = new ParameterCenter();
        }
        return obj;
    }

    // 手动输入的加工数据以及参数
    void setInputData(const QByteArray& pData);
    const QByteArray& getInputData();

    // 网络相关参数
    void setLocalPort(quint16 port);
    quint16 getLocalPort();
    void setRemotePort(quint16 port);
    quint16 getRemotePort();
    void setLocalIP(const QHostAddress &addr);
    QHostAddress getLocalIP();
    void setRemoteIP(const QHostAddress &addr);
    QHostAddress getRemoteIP();

private:
    QByteArray inputData;
    quint16 localPort;
    quint16 remotePort;
    QHostAddress localIP;
    QHostAddress remoteIP;
};

#endif // PARAMETERCENTER_H
