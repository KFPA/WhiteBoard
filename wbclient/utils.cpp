#include "utils.h"
#include "QPixmap"
#include "QApplication"
#include "QDesktopWidget"
#include "QUrl"
#include "QDesktopServices"
#include "QJsonObject"
#include "QJsonDocument"

namespace wb{

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


}
