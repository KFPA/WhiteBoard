#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(600, 480);

    QString Application = QApplication::applicationDirPath();
    Application = ":/res/bing.ico";
    w.setWindowIcon(QIcon(Application));
    w.setWindowTitle("电子白板");
    w.show();
    return a.exec();
}
