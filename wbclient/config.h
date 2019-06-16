#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <QString>
#include <QSettings>
#include "singleton.h"

namespace wb{

    class Config
    {
     public:
        Config();
        ~Config();
    public:
        QString getServerIp();

        int32_t getServerPort();

        void setGeometry(QByteArray geometry);

        QByteArray getGeometry();

        void setState(QByteArray state);

        QByteArray getState();
    protected:
        void init();
        void uinit();
    private:
        QString _strPath;

        QSettings* _pConfig;
    };












}


#endif // __CONFIG_H__
