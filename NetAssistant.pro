#-------------------------------------------------
#
# Project created by QtCreator 2015-06-27T22:18:28
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetAssistant
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        mainwindow.cpp \
    udpclient.cpp \
    commonhelper.cpp \
    tcpclient.cpp \
    tcpclientserver.cpp \
    tcpserver.cpp \
    netmanager.cpp \
    DataOutputShow.cpp

HEADERS  += mainwindow.h \
    udpclient.h \
    commonhelper.h \
    abstractclient.h \
    tcpclient.h \
    tcpclientserver.h \
    tcpserver.h \
    netmanager.h \
    BaseDef.h \
    DataOutputShow.h

FORMS    += mainwindow.ui
