#ifndef PARAMETERCENTER_H
#define PARAMETERCENTER_H

#include <QObject>
#include <QHostAddress>

/**
 * @brief The ParameterCenter class
 * @author yxj
 * @date 2018/12/10
 * @details ����Ϊ����ģʽ����Ϊ����Ĳ������ģ��ṩ������ȡ�Ľӿڸ�����ĸ���ģ��
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

    // �ֶ�����ļӹ������Լ�����
    void setInputData(const QByteArray& pData);
    const QByteArray& getInputData();

    // ������ز���
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
