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
    };

}
#endif // UTILS_H
