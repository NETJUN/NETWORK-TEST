#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUdpSocket>
#include "udpclient.h"
#include <QLabel>
#include <QDate>
#include <QStringList>
#include "commonhelper.h"
#include "DataOutputShow.h"
#include "ParameterCenter.h"
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    qDebug("%s", __func__);
    ui->setupUi(this);

    /* 读取配置文件 */
    doSettings(false);

    // udp性能测试
    file = nullptr;
    udpDataFlag = false;

    /* 设置默认通讯模式 */
    ui->tcpclient_radioButton->setChecked(true);
    /** 目前设置为UDP为默认方式 */
    ui->udp_radioButton->setChecked(true);
    /** 设置远程主机IP地址 获取本机IP */
    ui->remoteIP_lineEdit->setText(mRemoteIp);
    /* 设置远程端口号 */
    /* TODO: 将其设置为不能以0开头 */
    ui->remoteport_spinBox->setRange(1024,9999);
    ui->remoteport_spinBox->setValue(mRemotePort);
    /* 设置本地端口号 */
    ui->localport_spinBox->setRange(1024,9999);
    ui->localport_spinBox->setValue(mLocalPort);

    connected = false;
    // 初始化actions
    createActions();
    // 初始化菜单
    creatMenus();
    // 初始化状态栏
    initStatusBarWidget();
    // 创建网络管理对象
    netManager = new NetManager();
    connect(netManager, SIGNAL(TCPConnectedSignal()), this, SLOT(TCPConnected()));
    // 更新接收到的数据
    connect(netManager, SIGNAL(messageReceivedSignal(QString)), this, SLOT(updateReceiveText(QString)));
    connect(netManager, SIGNAL(updateDataStatusSignal(QString, QVariant, QVariant)), this, SLOT(updateStateBar(QString, QVariant, QVariant)));
    connect(DataOutputShow::getDataOutputInstance(), SIGNAL(messageReceivedSignal(QString)), this, SLOT(updateReceiveText(QString)));
    connect(DataOutputShow::getDataOutputInstance(), SIGNAL(updateDataStatusSignal(QString,QVariant,QVariant)), this, SLOT(updateStateBar(QString,QVariant,QVariant)));

    connect(ui->remoteIP_lineEdit, SIGNAL(returnPressed()), this, SLOT(ipLineEditSlot()));
    connect(ui->remoteport_spinBox, SIGNAL(valueChanged(int)), this, SLOT(localPortSpinBoxSlot(int)));
    connect(ui->localport_spinBox, SIGNAL(valueChanged(int)), this, SLOT(remotePortSpinBoxSlot(int)));
    connect(ui->send_plainTextEdit, SIGNAL(textChanged()), this, SLOT(sendPlainTextEditSlot()));
    init();
    mReceiveNum = mSendNum = 0;
    mLocalIp = commonHelper.getLocalHostIP().toString();
}

void MainWindow::connectNet() {
    qDebug("%s", __func__);

    mRemoteIp = ui->remoteIP_lineEdit->text();
    if(mRemoteIp.isEmpty()){
        updateStateBar(QString::fromUtf8("未指定IP地址"), QVariant(QVariant::Int), QVariant(QVariant::Int));
        return;
    }
    mRemotePort = ui->remoteport_spinBox->text().toInt();
    mLocalPort = ui->localport_spinBox->text().toInt();


    netManager->setIpAndPort(QHostAddress(mRemoteIp), mRemotePort, QHostAddress(mLocalIp), mLocalPort);

    if(ui->udp_radioButton->isChecked()) {
        updateStateBar(QString::fromLocal8Bit("UDP通信 ") + mRemoteIp + ":" + QString().number(mRemotePort),
                       QVariant(QVariant::Int), QVariant(QVariant::Int));
        // 开启UDP通信
        netManager->UDPStart();
        mode = UDP_MODE;
    } else if(ui->tcpclient_radioButton->isChecked()){
        updateStateBar(QString::fromLocal8Bit("TCP通信 ") + mRemoteIp + ":" + QString().number(mRemotePort)
                       + QString::fromLocal8Bit(" 连接中..."), QVariant(QVariant::Int), QVariant(QVariant::Int));
        netManager->TCPConnectToHost();
        mode = TCP_CLIENT_MODE;
    } else {
        updateStateBar(QString::fromLocal8Bit("TCP服务器") + QHostAddress::LocalHost + QString().number(mLocalPort)
                       + QString::fromLocal8Bit(" 监听中..."), QVariant(QVariant::Int), QVariant(QVariant::Int));
        netManager->startListen();
        mode = TCP_SERVER_MODE;
    }

    connected = true;
    // 将状态设置为 通
    ui->state_label->setText(QString::fromLocal8Bit("通"));
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::blue);
    ui->state_label->setPalette(pa);

    // 将按钮设置为　断开网络
    ui->connect_pushButton->setText(QString::fromLocal8Bit("断开网络"));

    // 禁用远程端口，本地端口，远程IP
    ui->remoteIP_lineEdit->setEnabled(false);
    ui->remoteport_spinBox->setEnabled(false);
    ui->localport_spinBox->setEnabled(false);
    // 使能button
    ui->handSend_pushButton->setEnabled(true);
}

void MainWindow::updateReceiveText(const QString string) {
    udpRecvData = string;
    udpDataFlag = true;
}

/**
 * @brief MainWindow::updateStateBar
 * @param state 状态
 * @param inNum 接收数值
 * @param outNum 发送数值
 */
void MainWindow::updateStateBar(QString state, QVariant inNum, QVariant outNum) {
    if(!state.isNull())
        statusLabel->setText(state);

    if(!inNum.isNull()) {
        //累计
        if(inNum.toInt() == 0)
            mReceiveNum = 0;
        else
            mReceiveNum += inNum.toInt();
        receiveLabel->setText(QString::fromLocal8Bit("接收:") + QString::number(mReceiveNum));
    }

    if(!outNum.isNull()) {
        //累计
        if(outNum.toInt() == 0)
            mSendNum = 0;
        else
            mSendNum += outNum.toInt();
        sendLabel->setText(QString::fromLocal8Bit("发送:") + QString::number(mSendNum));
    }

}

/**
 * 断开UDP时调用该函数
 * @brief MainWindow::init
 */
void MainWindow::init() {
    qDebug("%s", __func__);
    // No.1
    connected = false;
    // 将状态设置为 断
    ui->state_label->setText(QString::fromLocal8Bit("断"));
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->state_label->setPalette(pa);

    // 将按钮设置为　连接网络
    ui->connect_pushButton->setText(QString::fromLocal8Bit("连接网络"));

    // 使能远程端口，本地端口，远程IP
    ui->remoteIP_lineEdit->setEnabled(true);
    ui->remoteport_spinBox->setEnabled(true);
    ui->localport_spinBox->setEnabled(true);
    // 禁用button
    ui->handSend_pushButton->setEnabled(false);
    // 确保关闭UDP端口
    netManager->UDPStop();
    updateStateBar(QString::fromLocal8Bit("本地IP: ") + commonHelper.getLocalHostIP().toString() + QString::fromLocal8Bit(" 无连接"),
                   QVariant(QVariant::Int), QVariant(QVariant::Int));
}

void MainWindow::udpNetTest() {
    QByteArray data;
    if(file == nullptr) {
        qDebug() << "file is empty!!!";
        return;
    }
    data = file->read(100);
    while(data.size() != 0) {
        netManager->dataSend(file->read(100), mode);
        while(!udpDataFlag) {
            QThread::msleep(1);
        }
        if(data != udpRecvData) {
            qDebug() << "file check failed!";
            return;
        }
        data = file->read(100);
    }
    file->close();
    qDebug() << "file check success!";
}

void MainWindow::createActions() {
    openfile = new QAction(QIcon(":/images/res/open.png"), tr("&openFile"), this);
    openfile->setShortcut(tr("Ctrl+O"));
    openfile->setToolTip(tr("Open a file"));
    openfile->setStatusTip(tr("Open a file"));
    connect(openfile, SIGNAL(triggered()), this, SLOT(openFile()));
}

void MainWindow::creatMenus() {
    ui->menuYywr->addAction(openfile);
}


/**
 * 断开UDP时调用该函数
 * @brief MainWindow::disConnectNet
 */
void MainWindow::disConnectNet() {
    qDebug("%s", __func__);
    // No.1
    connected = false;
    // 将状态设置为 断
    ui->state_label->setText(QString::fromLocal8Bit("断"));
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->state_label->setPalette(pa);

    // 将按钮设置为　连接网络
    ui->connect_pushButton->setText(QString::fromLocal8Bit("连接网络"));

    // 使能远程端口，本地端口，远程IP
    ui->remoteIP_lineEdit->setEnabled(true);
    ui->remoteport_spinBox->setEnabled(true);
    ui->localport_spinBox->setEnabled(true);
    // 禁用button
    ui->handSend_pushButton->setEnabled(false);
    // 断开连接
    netManager->disConnectNet(mode);

    updateStateBar(QString::fromLocal8Bit("连接断开"), QVariant(QVariant::Int), QVariant(QVariant::Int));
}

void MainWindow::doSettings(bool isWrite)
{
    QSettings settings("Yzs_think", "Application");
    const QString REMOTE_IP = "remoteip";
    const QString REMOTE_PORT = "remoteport";
    const QString LOCAL_PORT = "localport";
    if(isWrite) {
        settings.setValue(REMOTE_IP, mRemoteIp);
        settings.setValue(REMOTE_PORT, mRemotePort);
        settings.setValue(LOCAL_PORT, mLocalPort);
    } else {
        mRemoteIp = settings.value(REMOTE_IP, commonHelper.getLocalHostIP().toString()).toString();
        mRemotePort = settings.value(REMOTE_PORT, 1234).toInt();
        mLocalPort = settings.value(LOCAL_PORT, 2468).toInt();
    }
    ParameterCenter::getParameterCenterInstance()->setLocalPort(mLocalPort);
    ParameterCenter::getParameterCenterInstance()->setRemotePort(mRemotePort);
    ParameterCenter::getParameterCenterInstance()->setRemoteIP(QHostAddress(mRemoteIp));
}

MainWindow::~MainWindow()
{
    doSettings(true);
    delete netManager;
    delete ui;
}

/**
 * @brief MainWindow::on_clearCounter_pushButton_released
 * 该函数是将计数器置零
 */
void MainWindow::on_clearCounter_pushButton_released() {
    qDebug("%s", __func__);
    updateStateBar(QString(), 0, 0);
}

void MainWindow::on_clearReceive_pushButton_released() {
    ui->receive_textBrowser->clear();
}

void MainWindow::on_clearSend_pushButton_released() {
    ui->send_plainTextEdit->clear();
}

/**
 * @brief MainWindow::on_connect_pushButton_released
 * 该函数是在连接/断开网络时调用
 */
void MainWindow::on_connect_pushButton_released() {
    qDebug("%s", __func__);
    // 如果当前网络是连接状态　调用断开连接函数
    if(connected) {
        disConnectNet();
    } else { // 否则调用连接函数
        connectNet();
    }
}

/**
 * 当用户点击 发送 时调用该函数
 * @brief MainWindow::on_handSend_pushButton_released
 */
void MainWindow::on_handSend_pushButton_released() {
    // 获取 rmeote ip/ port 和内容
    QString string = ui->send_plainTextEdit->toPlainText();
//    if(string.length() != 0) {
//        QByteArray data;
//        QStringList tmp = string.split(" ");
//        bool ok;
//        for(auto itr = tmp.begin(); itr != tmp.end(); ++itr) {
//            data.push_back(itr->toUInt(&ok, 16));
//        }
//        netManager->dataSend(data, mode);
//    }
    //netManager->dataSend(string.toStdString().c_str(), mode);
    udpNetTest();
}

void MainWindow::on_quit_pushButton_released() {
    QApplication::quit();
}

void MainWindow::ipLineEditSlot() {
    QString text = ui->remoteIP_lineEdit->text();
    ParameterCenter::getParameterCenterInstance()->setRemoteIP(QHostAddress(text));
}

void MainWindow::localPortSpinBoxSlot(int value) {
    ParameterCenter::getParameterCenterInstance()->setLocalPort(value);
}

void MainWindow::remotePortSpinBoxSlot(int value) {
    ParameterCenter::getParameterCenterInstance()->setRemotePort(value);
}

void MainWindow::sendPlainTextEditSlot() {
    QString string = ui->send_plainTextEdit->toPlainText();
    ParameterCenter::getParameterCenterInstance()->setInputData(string.toStdString().c_str());
}

void MainWindow::initStatusBarWidget() {
    // 状态栏
    statusLabel = new QLabel;
    statusLabel->setMinimumSize(260, 20); // 设置标签最小大小
    statusLabel->setFrameShape(QFrame::WinPanel); // 设置标签形状
    statusLabel->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(statusLabel);
    statusLabel->setText(QString::fromLocal8Bit("UDP通信停止"));
    statusLabel->setAlignment(Qt::AlignHCenter);

    // 接收数量
    receiveLabel = new QLabel;
    receiveLabel->setMinimumSize(150, 20); // 设置标签最小大小
    receiveLabel->setFrameShape(QFrame::WinPanel); // 设置标签形状
    receiveLabel->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(receiveLabel);
    receiveLabel->setAlignment(Qt::AlignHCenter);

    // 发送数量
    sendLabel = new QLabel;
    sendLabel->setMinimumSize(150, 20); // 设置标签最小大小
    sendLabel->setFrameShape(QFrame::WinPanel); // 设置标签形状
    sendLabel->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(sendLabel);
    sendLabel->setAlignment(Qt::AlignHCenter);
    updateStateBar(QString(), 0, 0);

    // 计数器清零 button
    clearCounterButton = new QPushButton();
    ui->statusBar->addWidget(clearCounterButton);
    clearCounterButton->setText(QString::fromLocal8Bit("计数器清零"));
    connect(clearCounterButton, SIGNAL(released()), this, SLOT(on_clearCounter_pushButton_released()));

    // 时间 TODO:要进行更新
    timeLabel = new QLabel;
//    timeLabel->setMinimumSize(90, 20); // 设置标签最小大小
//    timeLabel->setMaximumWidth(120);
    timeLabel->setFrameShape(QFrame::WinPanel); // 设置标签形状
    timeLabel->setFrameShadow(QFrame::Sunken); // 设置标签阴影
    ui->statusBar->addWidget(timeLabel);
    timeLabel->setText(QDate::currentDate().toString("yyyy-MM-dd") + " " + QDateTime::currentDateTime().toString("hh:mm"));
}

void MainWindow::TCPConnected() {
    ui->state_label->setText(QString::fromLocal8Bit("已连接"));
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::green);
    ui->state_label->setPalette(pa);

    // 将按钮设置为　断开网络
    ui->connect_pushButton->setText(QString::fromLocal8Bit("断开网络"));

    // 禁用远程端口，本地端口，远程IP
    ui->remoteIP_lineEdit->setEnabled(false);
    ui->remoteport_spinBox->setEnabled(false);
    ui->localport_spinBox->setEnabled(false);
    // 使能button
    ui->handSend_pushButton->setEnabled(true);
}

void MainWindow::openFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select A File", "/", "DXF File(*.dxf)");
    if(filePath.isEmpty()) {
        QMessageBox::information(this, tr("Information"), tr("Open failed"));
        return;
    }
    file = new QFile(filePath);
    bool ok = file->open(QIODevice::ReadOnly);
    if(!ok) {
        file = nullptr;
        return;
    }
}
