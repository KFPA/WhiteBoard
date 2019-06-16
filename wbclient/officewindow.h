#ifndef OFFICEWINDOW_H
#define OFFICEWINDOW_H


#include <ActiveQt/QAxWidget>
#include <ActiveQt/QAxObject>


namespace wb{
    class OfficeWindow : public QAxWidget
    {
    public:
        OfficeWindow(QWidget *parent = nullptr);
        ~OfficeWindow();
    public:
        bool showOffice(QString strPath);
    private:
        QString _docPath;
    };
}


#endif // OFFICEWINDOW_H
