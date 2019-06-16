#include "officewindow.h"
#include <QDir>

#if defined (_MSC_VER)&&(MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

namespace wb{

//    OfficeWindow::OfficeWindow(QWidget *parent = nullptr):QAxWidget (parent)
//    {
//        this->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
//        showOffice(_docPath);
//    }

    OfficeWindow::OfficeWindow(QWidget *parent) : QAxWidget (parent,nullptr)
    {
        this->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
        showOffice(_docPath);
    }

    OfficeWindow::~OfficeWindow()
    {

    }

    bool OfficeWindow::showOffice(QString strPath)
    {
        if(strPath.isEmpty()){
           return false;
        }
        _docPath = strPath;
        QVariant ret = this->dynamicCall("Navigate(const QString&)",QDir::toNativeSeparators(strPath));
        return ret.isValid();

    }

}


