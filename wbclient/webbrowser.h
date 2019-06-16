#ifndef WEBBROWSER_H
#define WEBBROWSER_H


#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QWidget>
#include "officewindow.h"
#include "utils.h"
#include "mainwindow.h"

namespace wb{

    enum DocMode{
        NONE_DOC = 0,    //!< 默认模式
        SHOW_DOC = 1,    //!< 显示模式
        POSTIL_DOC = 2   //!< 批注模式
    };

    class WebBrowser : public QWidget
    {
         Q_OBJECT
    public:
        explicit WebBrowser();
        ~WebBrowser() override;


    public:


        /**
         * @brief 设置显示文档
         * @param strPath 显示文档的绝对路径
         * */
        bool showOffice(QString strPath);

         void setInfo(WBInfo wbInfo){_wbInfo = wbInfo;}
    protected:
        void mouseMoveEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent *paint) override;
        bool eventFilter(QObject *obj, QEvent *event) override;

    protected:
        void init();
        void uint();

        void initCtrlWidget();

        void initOfficeWidget();


        void resizeCtrlWidget();
        void checkCtrlWidgetRect(QPoint point);


    private:
        QWidget*            _ctrlsWidget;
        OfficeWindow*       _officeWidget;
        QPropertyAnimation* _ctrlOpenAnimation;
        QPropertyAnimation* _ctrlCloseAnimation;

        QPixmap    _pixmap;
        DocMode    _docMode;
        WBInfo     _wbInfo;
        MainWindow mainWin;
    signals:

    public slots:
        void closeDisplay();
        void postilOffice();
    };









}


#endif // WEBBROWSER_H
