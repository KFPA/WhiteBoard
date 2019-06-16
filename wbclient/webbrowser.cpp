#include <QDir>
#include "webbrowser.h"
#include <QBoxLayout>
#include "utils.h"
#include <QPushButton>
#include <QTime>
#include <QDebug>
#include <QPainter>




namespace wb {

    WebBrowser::WebBrowser() : QWidget(),_ctrlsWidget(nullptr),_officeWidget(nullptr),_ctrlOpenAnimation(nullptr),_ctrlCloseAnimation(nullptr),_docMode(SHOW_DOC)
    {
         init();
    }

    WebBrowser::~WebBrowser()
    {
        uint();
    }

    void WebBrowser::mouseMoveEvent(QMouseEvent *event)
    {
        qDebug() << QTime::currentTime() << __FUNCTION__;
        checkCtrlWidgetRect(event->pos());
    }

    void WebBrowser::paintEvent(QPaintEvent *paint)
    {
        Q_UNUSED(paint);
        resizeCtrlWidget();

        QPainter painter(this);
        painter.setPen(Qt::blue);
        painter.setFont(QFont("Arial", 30));
        if(_pixmap.isNull())
        {
            painter.drawText(rect(), Qt::AlignCenter, "WAIT FOR DATA");
        }
        else
        {
            painter.drawPixmap(QRect(0, 0, _pixmap.width(), _pixmap.height()), _pixmap.scaled(this->width(), this->height()));
        }
    }

    void WebBrowser::init()
    {
         initOfficeWidget();

         initCtrlWidget();
    }

    void WebBrowser::uint()
    {
        SAFE_DELETE(_ctrlsWidget);
        SAFE_DELETE(_ctrlCloseAnimation);
        SAFE_DELETE(_ctrlOpenAnimation);
    }

    bool WebBrowser::showOffice(QString strPath)
    {
        return _officeWidget->showOffice(strPath);
    }

    void WebBrowser::initCtrlWidget()
    {
        _ctrlsWidget = new QWidget(this);
        _ctrlsWidget->setWindowFlags(_ctrlsWidget->windowFlags() | Qt::FramelessWindowHint);
        _ctrlsWidget->close();
        _ctrlsWidget->setAutoFillBackground(true);
        _ctrlsWidget->setStyleSheet("background-color:#37474F");


        _ctrlOpenAnimation = new QPropertyAnimation(_ctrlsWidget, "geometry", this);
        _ctrlCloseAnimation = new QPropertyAnimation(_ctrlsWidget, "geometry", this);

        QVBoxLayout *layout = new QVBoxLayout(_ctrlsWidget);
        _ctrlsWidget->setLayout(layout);
        QPushButton *closeBtn = new QPushButton(_ctrlsWidget);

        closeBtn->setMinimumSize(72, 72);
        closeBtn->setStyleSheet("image:url(:/res/shutdown.png);\
                                 background:transparent;");

        layout->addWidget(closeBtn);
        connect(closeBtn, &QPushButton::clicked, this, &WebBrowser::closeDisplay);

        QPushButton* postilBtn = new QPushButton(_ctrlsWidget);
        assert(postilBtn);
        postilBtn->setMinimumSize(72,72);
        postilBtn->setStyleSheet("image:url(:/res/postil.png);\
                                 background:transparent;");
        layout->addWidget(postilBtn);
        connect(postilBtn,&QPushButton::clicked,this,&WebBrowser::postilOffice);
        this->setMouseTracking(true);
        _ctrlsWidget->installEventFilter(this);
    }

    void WebBrowser::initOfficeWidget()
    {
        _officeWidget = new OfficeWindow(this);
        _officeWidget->setWindowFlag(Qt::FramelessWindowHint);
        _officeWidget->setMinimumSize(Utils::deskRect().width()-1,Utils::deskRect().height());
    }

    void WebBrowser::resizeCtrlWidget()
    {
        int h = 200;
        int w = 100;
        _ctrlsWidget->setFixedSize(w, h);
    }

    void WebBrowser::checkCtrlWidgetRect(QPoint point)
    {
        // calculate current active rect
        QRect rect;
        rect.setLeft(this->width() - _ctrlsWidget->width());
        rect.setTop((this->height() - _ctrlsWidget->height()) / 2);
        rect.setWidth(_ctrlsWidget->width());
        rect.setHeight(_ctrlsWidget->height());


        // check wheather mousen is enter active rect
        bool bisShow = false;


        if(rect.contains(point))
        {
            bisShow = true;
        }

        QRect showRect(rect);
        QRect hideRect(QRect(showRect.right(), showRect.y(), 0, showRect.height()));

        if(bisShow)
        {
            // show control widget
            _ctrlsWidget->show();
            _ctrlOpenAnimation->setDuration(200);
            _ctrlOpenAnimation->setStartValue(hideRect);
            _ctrlOpenAnimation->setEndValue(showRect);
            _ctrlOpenAnimation->start();
        }
        else
        {
            if(_ctrlsWidget->isVisible())
            {

                _ctrlCloseAnimation->setDuration(200);
                _ctrlCloseAnimation->setStartValue(showRect);
                _ctrlCloseAnimation->setEndValue(hideRect);
                _ctrlCloseAnimation->start();

                connect(_ctrlCloseAnimation, &QPropertyAnimation::finished, _ctrlsWidget, &QWidget::close);
            }
        }
    }

    void WebBrowser::closeDisplay()
    {
        this->close();
    }

    void WebBrowser::postilOffice()
    {
        mainWin.setInfo(_wbInfo);
        mainWin.Join();
        mainWin.setWindowFlag(Qt::FramelessWindowHint);
        mainWin.setWindowIcon(QIcon(":/res/bing.ico"));
        mainWin.setWindowTitle("电子白板");
        mainWin.showMaximized();
        mainWin.raise();
        mainWin.activateWindow();
    }

    bool WebBrowser::eventFilter(QObject *obj, QEvent *event)
    {
        qDebug() << obj << QTime::currentTime() << event->type();
        if(event->type() == QEvent::MouseMove || (obj == _ctrlsWidget && event->type() == QEvent::Leave))
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            qDebug() << "event filter: " << mouseEvent->pos();
            checkCtrlWidgetRect(mouseEvent->pos());
            return true;
        }
        return false;
    }





}


