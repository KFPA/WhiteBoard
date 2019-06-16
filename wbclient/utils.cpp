#include "utils.h"
#include "QPixmap"
#include "QApplication"
#include "QDesktopWidget"
#include "QUrl"
#include "QDesktopServices"
#include "QJsonObject"
#include "QJsonDocument"
#include <stdlib.h>
#include <stdio.h>
#include <config.h>

namespace wb{

    static const char* PORT = "--port";
    static const char* NAME = "--name";
    static const char* MODE = "--mode";
    static const char* SHOW = "show";
    static const char* POSTIL = "postil";
    static const char* PATH = "--path";
    static const char* IP = "--ip";

    Utils::Utils()
    {

    }

    Utils::~Utils()
    {

    }

    QImage Utils::captureDesk()
    {
       QPixmap pixMap =  QPixmap::grabWindow(QApplication::desktop()->winId());
       return pixMap.toImage();
    }

    QRect Utils::deskRect()
    {
        QRect rect;
        QDesktopWidget* desktopWidget = QApplication::desktop();
        if(desktopWidget){
            rect = desktopWidget->screenGeometry();
        }
        return rect;
    }

    bool Utils::openFile(QString path)
    {
        return QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }


    QString Utils::qJsonObj2qStr(const QJsonObject &jsonObject)
    {
         return QString(QJsonDocument(jsonObject).toJson());
    }

    WBInfo Utils::parseCommand(int argc, char *argv[])
    {
        WBInfo wbInfo;
        wbInfo._name = argv[1];
        for(int32_t i = 1; i< argc; ++i){
            if(QString(argv[i]) == NAME && i+1 < argc){
                printf(argv[i]);
                wbInfo._name = argv[++i];
            }
            else if(QString(argv[i]) == MODE && i+1 < argc){
                if(QString(argv[i+1]) == SHOW){
                    wbInfo._mode = SHOW_MODE;
                }
                else if(QString(argv[i+1]) == POSTIL){
                    wbInfo._mode = POSTILL_MODE;
                }
                else{
                    wbInfo._mode = NONE_MODE;
                }
            }
            else if(QString(argv[i]) == PATH && i+1 < argc){
                wbInfo._path = argv[++i];
            }
            else if(QString(argv[i]) == IP && i+1 < argc){
                wbInfo._ip = argv[++i];
            }
            else if(QString(argv[i]) == PORT && i+1 < argc){
                wbInfo._port = atoi(argv[++i]);
            }
        }
        if(wbInfo._ip.isEmpty()){
           wbInfo._ip = wb::singleton<wb::Config>::GetInstance()->getServerIp();
        }
        wbInfo._port = wb::singleton<wb::Config>::GetInstance()->getServerPort();
        return wbInfo;

    }


}
