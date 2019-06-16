#-------------------------------------------------
#
# Project created by QtCreator 2018-08-25T10:04:16
#
#-------------------------------------------------

QT       += core gui network
QT       += axcontainer
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wbclient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    painterscene.cpp \
    painterview.cpp \
    shapes.cpp \
    whiteboardconnection.cpp \
    utils.cpp \
    fileinfos.cpp \
    config.cpp \
    webbrowser.cpp \
    officewindow.cpp

HEADERS  += mainwindow.h \
    painterscene.h \
    painterview.h \
    shapes.h \
    whiteboardconnection.h \
    utils.h \
    fileinfos.h \
    singleton.h \
    config.h \
    data_define.h \
    webbrowser.h \
    officewindow.h




RESOURCES += \
    icons.qrc \
    images.qrc

include(Common/common.pri)
include(HttpRequest/httpRequest.pri)
