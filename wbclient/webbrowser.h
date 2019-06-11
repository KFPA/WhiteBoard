#ifndef WEBBROWSER_H
#define WEBBROWSER_H



#include <ActiveQt/QAxWidget>
#include <ActiveQt/QAxObject>


namespace wb{

    enum DocMode{
        NONE_DOC = 0,    //!< 默认模式
        SHOW_DOC = 1,    //!< 显示模式
        POSTIL_DOC = 2   //!< 批注模式
    };

    class WebBrowser : public QAxWidget
    {
    public:
        WebBrowser(QString strPath);
        ~WebBrowser();


    public:


        /**
         * @brief 设置显示文档
         * @param strPath 显示文档的绝对路径
         * */
        void setDocPath(QString strPath);

    protected:
        void init();
        void uint();

        bool show(QString strPath);
    private:
        DocMode    _docMode;
        QString    _docPath;
    };









}


#endif // WEBBROWSER_H
