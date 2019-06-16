#include "config.h"
#include <QDir>
#include "utils.h"
#include <QMessageBox>
#include <QDebug>

namespace wb{

const static char* SERVER_IP    = "server/ip";
const static char* SERVER_PORT  = "server/port";
const static char* WIN_GEOMETRY = "window/geometry";
const static char* WIN_STATE    = "window/state";

    Config::Config():_pConfig(nullptr)
    {
        init();
    }

    Config::~Config()
    {
        uinit();
    }

    void Config::init()
    {
        _strPath = QDir::currentPath() + "/config/config.ini";
        qDebug() << _strPath;
        if(!QFile::exists(_strPath)){
           QMessageBox::information(nullptr, "Information","Missing configuration file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
        _pConfig = new QSettings(_strPath,QSettings::IniFormat);
    }

    void Config::uinit()
    {
        SAFE_DELETE(_pConfig);
    }

    QString Config::getServerIp()
    {
        QString serverIp;
        if(_pConfig != nullptr){
            serverIp = _pConfig->value(SERVER_IP,"127.0.0.1").toString();
        }
        return serverIp;
    }

    int32_t Config::getServerPort()
    {
        int32_t serverPort;
        if(_pConfig != nullptr){
            serverPort = _pConfig->value(SERVER_PORT,"9001").toInt();
        }
        return serverPort;
    }

    void Config::setGeometry(QByteArray geometry)
    {
        if(_pConfig != nullptr){
            _pConfig->setValue(WIN_GEOMETRY,geometry);
        }
    }

    QByteArray Config::getGeometry()
    {
        QByteArray byte;
        if(_pConfig != nullptr){
            byte = _pConfig->value(WIN_GEOMETRY).toByteArray();
        }
        return byte;
    }

    void Config::setState(QByteArray state)
    {
        if(_pConfig != nullptr){
            _pConfig->setValue(WIN_STATE,state);
        }
    }

    QByteArray Config::getState()
    {
        QByteArray byte;
        if(_pConfig != nullptr){
            byte = _pConfig->value(WIN_STATE).toByteArray();
        }
        return byte;
    }

}

