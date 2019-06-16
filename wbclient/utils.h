#ifndef UTILS_H
#define UTILS_H

#include "QImage"
namespace wb{
    enum WBMode{
        NONE_MODE = 0,
        SHOW_MODE = 1,
        POSTILL_MODE = 2,
        MODE_NUM
    };

    struct WBInfo{
        QString _name;
        QString _path;
        QString _ip;
        int32_t _port = 9001;
        WBMode  _mode = NONE_MODE;
        bool isValid(){
            bool bIsValid = false;
            if(!_name.isNull() && (_mode > NONE_MODE || _mode < MODE_NUM) && (!_ip.isNull())){
                switch (_mode) {
                case SHOW_MODE:{
                    bIsValid = !_path.isNull();
                    break;
                }
                case POSTILL_MODE:
                {
                    bIsValid = true;
                }
                    break;
                default:
                    break;
                }
            }
            return bIsValid;
        }


    };


#define SAFE_DELETE(a){if(a){delete (a);((a)=nullptr);}}

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

        static WBInfo parseCommand(int argc, char *argv[]);



    };




}
#endif // UTILS_H
