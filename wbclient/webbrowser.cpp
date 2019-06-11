
#include "webbrowser.h"

#include <Qdir>
#include <Qurl>



namespace wb {

#if defined (_MSC_VER)&&(MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif


    WebBrowser::WebBrowser(QString strPath) : QAxWidget(nullptr,nullptr),_docMode(SHOW_DOC),_docPath(strPath)
    {
       init();
    }

    WebBrowser::~WebBrowser()
    {
        uint();
    }

    void WebBrowser::setDocPath(QString strPath)
    {
        _docPath = strPath;
        show(strPath);
    }

    void WebBrowser::init()
    {
         this->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
         show(_docPath);
    }

    void WebBrowser::uint()
    {

    }

    bool WebBrowser::show(QString strPath)
    {
        QVariant ret = this->dynamicCall("Navigate(const QString&)",QDir::toNativeSeparators(strPath));
        return ret.isValid();

    }





}


