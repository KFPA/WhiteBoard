#ifndef UTILS_H
#define UTILS_H

#include "QImage"
namespace wb{

    class Utils
    {
    public:
        Utils();
        ~Utils();
    public:
        static QImage captureDesk();

        static QRect deskRect();

        static bool openFile(QString path);

        static bool jsonTest();

        static QString qJsonObj2qStr(const QJsonObject& jsonObject);

    };

}
#endif // UTILS_H
