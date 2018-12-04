#ifndef DATAOUTPUTSHOW_H
#define DATAOUTPUTSHOW_H
#include <QObject>
#include <QVariant>

/*
 * 单例类
 * 用于输出数据
 */
class DataOutputShow : public QObject {
    Q_OBJECT
private:
    DataOutputShow() {};
    static DataOutputShow* obj;

public:
    static DataOutputShow* getDataOutputInstance() {
        if(obj == NULL) {
            obj = new DataOutputShow();
        }
        return obj;
    }

signals:
    void messageReceivedSignal(QString msg);
    void updateDataStatusSignal(QString status, QVariant inNum, QVariant outNum);
};

#endif // DATAOUTPUTSHOW_H
