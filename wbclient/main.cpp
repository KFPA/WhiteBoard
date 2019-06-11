#include "mainwindow.h"
#include <QApplication>
#include "fileinfos.h"
#include "string"
#include "QDebug"
#include "webbrowser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // wb::MainWindow w;
    wb::WebBrowser w;
    w.setMinimumSize(600, 480);




    QString Application = QApplication::applicationDirPath();
    w.setWindowIcon(QIcon(":/res/bing.ico"));
    w.setWindowTitle("电子白板");
    w.show();
    w.raise();
    w.activateWindow();


    return a.exec();
}
