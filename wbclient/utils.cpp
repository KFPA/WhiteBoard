#include "utils.h"
#include "QPixmap"
#include "QApplication"
#include "QDesktopWidget"

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


}
