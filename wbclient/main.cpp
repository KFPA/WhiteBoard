#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "fileinfos.h"
#include "string"
#include "QDebug"
#include "webbrowser.h"
#include "utils.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString Application = QApplication::applicationDirPath();
    wb::WBInfo wbInfo = wb::Utils::parseCommand(argc,argv);
    if(!wbInfo.isValid()){
        QMessageBox::critical(nullptr, "critical", "Input arguments error!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return -1000;
    }

    wb::WebBrowser webBrowser;
    wb::MainWindow mainWin;
    switch (wbInfo._mode) {
    case wb::SHOW_MODE:{
        webBrowser.setInfo(wbInfo);
        webBrowser.showOffice(wbInfo._path);
       // webBrowser.setWindowFlag(Qt::FramelessWindowHint);
        webBrowser.setAttribute(Qt::WA_TransparentForMouseEvents);
        webBrowser.setWindowIcon(QIcon(":/res/bing.ico"));
        webBrowser.setWindowTitle("电子白板");
        webBrowser.showMaximized();
        webBrowser.raise();
        webBrowser.activateWindow();
    }
        break;
    case wb::POSTILL_MODE:{
        mainWin.setInfo(wbInfo);
        mainWin.Join();
        mainWin.setWindowFlag(Qt::FramelessWindowHint);
        mainWin.setWindowIcon(QIcon(":/res/bing.ico"));
        mainWin.setWindowTitle("电子白板");
        mainWin.showMaximized();
        mainWin.raise();
        mainWin.activateWindow();
    }
        break;
    default:
        break;
    }



    return a.exec();
}
