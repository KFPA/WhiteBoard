#include "mainwindow.h"
#include <QtWidgets>
#include <QSizeF>
#include <QImage>
#include <QtDebug>
#include "painterscene.h"
#include "painterview.h"
#include "utils.h"
#include "webbrowser.h"
#include "config.h"


namespace wb{

    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), m_scene(nullptr)
        , m_conn(nullptr), m_toolBar(nullptr)
    {

    }

    MainWindow::~MainWindow()
    {

    }

    void MainWindow::preparePainterUI()
    {

        if(!m_toolBar)
        {

            QToolBar *toolbar = addToolBar("ToolBar");
            QActionGroup *actionGroup = new QActionGroup(toolbar);
            m_toolBar = toolbar;
            toolbar->setFloatable(true);

            toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            QAction *action = toolbar->addAction(QIcon(":/res/pen.png"),
                                        "Draw",
                                        this, SLOT(onDrawGraffitiAction()));
            action->setCheckable(true);
            action->setChecked(true);
            action->setActionGroup(actionGroup);

            action = toolbar->addAction(QIcon(":/res/line.png"),
                                                 "Line",
                                                 this, SLOT(onDrawLineAction()));
            action->setCheckable(true);         
            action->setActionGroup(actionGroup);

            action = toolbar->addAction(QIcon(":/res/rect.png"),
                                        "Rectangle",
                                        this, SLOT(onDrawRectangleAction()));
            action->setCheckable(true);
            action->setActionGroup(actionGroup);

            action = toolbar->addAction(QIcon(":/res/oval.png"),
                                        "Oval",
                                        this, SLOT(onDrawOvalAction()));
            action->setCheckable(true);
            action->setActionGroup(actionGroup);

            action = toolbar->addAction(QIcon(":/res/triangle.png"),
                                        "Triangle",
                                        this, SLOT(onDrawTriangleAction()));
            action->setCheckable(true);
            action->setActionGroup(actionGroup);



            toolbar->addSeparator();

            action = toolbar->addAction(QIcon(":/res/del.png"), "Back",
                                        this, SLOT(onUndo()));
            action->setActionGroup(actionGroup);

            action = toolbar->addAction(QIcon(":/res/clearall.png"), "Clear",
                                        this, SLOT(onClearAll()));
            action->setActionGroup(actionGroup);

            action = toolbar->addAction(QIcon(":/res/icon_empty.png"), "Save",
                                        this, SLOT(onSave()));
            action = toolbar->addAction(QIcon(":/res/close.png"),"Close",
                                        this, SLOT(onClose()));
            action->setActionGroup(actionGroup);

        }
        else
        {
            addToolBar(m_toolBar);
        }

        m_scene = new PainterScene();
        //m_scene->setBkImage(wb::Utils::captureDesk());
        m_scene->setToolType(tt_Graffiti);
        auto *view = new PainterView(m_scene);
        connect(m_scene, SIGNAL(addFigureReq(QJsonObject)),
                this, SLOT(onAddFigureReq(QJsonObject)));
        connect(m_scene, SIGNAL(deleteFigureReq(int)),
                this, SLOT(onDeleteFigureReq(int)));
        connect(m_scene, SIGNAL(clearFiguresReq(int)),
                this, SLOT(onClearFiguresReq(int)));

        setCentralWidget(view);
        restoreGeometry(singleton<Config>::GetInstance()->getGeometry());
        restoreState(singleton<Config>::GetInstance()->getState());
    }

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        singleton<Config>::GetInstance()->setGeometry(saveGeometry());
        singleton<Config>::GetInstance()->setState(saveState());
    }

    void MainWindow::onDrawLineAction()
    {
        if(m_scene){
             m_scene->setToolType(tt_Line);
        }

    }

    void MainWindow::onDrawRectangleAction()
    {
        if(m_scene){
             m_scene->setToolType(tt_Rectangle);
        }
    }

    void MainWindow::onDrawOvalAction()
    {
        if(m_scene){
             m_scene->setToolType(tt_Oval);
        }
    }

    void MainWindow::onDrawTriangleAction()
    {
        if(m_scene){
             m_scene->setToolType(tt_Triangle);
        }
    }

    void MainWindow::onDrawGraffitiAction()
    {
        if(m_scene){
              m_scene->setToolType(tt_Graffiti);
        }
    }

    void MainWindow::onUndo()
    {
        if(m_scene){
           m_scene->undo();
        }

    }

    void MainWindow::onClearAll()
    {
        if(m_conn){
            m_conn->clearFigures(-1);
        }
    }

    void MainWindow::onSave()
    {
        bool bRet = false;
        if(m_scene){
         QString strPath = QDir::currentPath()+"/data/postilRes.png";
         bRet = m_scene->save(strPath,"PNG");
        }
        if(!bRet){
           QMessageBox::information(nullptr, "Information", "Save failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
    }

    void MainWindow::onClose()
    {
        close();
    }

    bool MainWindow::Join()
    {
        if(!m_conn && m_wbInfo.isValid())
        {
            m_conn = new WBConnection(this);
            connect(m_conn, SIGNAL(joined(QString,int)),
                    this, SLOT(onJoined(QString,int)));
            connect(m_conn, SIGNAL(errorOccurred(const QString)),
                    this,SLOT(onErrorOccurred(const QString)));
            connect(m_conn, SIGNAL(userLeft(QString,int)),
                    this, SLOT(onUserLeft(QString,int)));
            connect(m_conn, SIGNAL(figureAdded(QJsonObject)),
                    this, SLOT(onFigureAdded(QJsonObject)));
            connect(m_conn, SIGNAL(figureDeleted(int)),
                    this, SLOT(onFigureDeleted(int)));
            connect(m_conn, SIGNAL(figureCleared(int)),
                    this, SLOT(onFiguresCleared(int)));
            m_conn->join(m_wbInfo._name, m_wbInfo._ip, m_wbInfo._port);
            return true;
        }
        return false;
    }



    void MainWindow::onJoined(QString name, int id)
    {
        if(id == m_conn->id())
        {
            setWindowTitle(QString("wbclient - %1").arg(name));
            preparePainterUI();
            m_scene->setUserId(id);
        }
        else
        {
            //TODO
        }
    }

    void MainWindow::onUserLeft(QString name, int id)
    {
        if(id == m_conn->id())
        {
            setWindowTitle("wbclient");
            m_scene = nullptr;
            removeToolBar(m_toolBar);
            Join();
        }
        else
        {
            //TODO
        }
    }

    void MainWindow::onFigureAdded(const QJsonObject &figure)
    {
        m_scene->onFigureAdded(figure);
    }

    void MainWindow::onFigureDeleted(int id)
    {
        m_scene->onFigureDeleted(id);
    }

    void MainWindow::onFiguresCleared(int ownerId)
    {
        m_scene->onFiguresCleared(ownerId);
    }

    void MainWindow::onErrorOccurred(const QString &desc)
    {
        if(m_scene)
        {
            m_scene = nullptr;
            removeToolBar(m_toolBar);
            Join();
        }
        if(m_conn)
        {
            m_conn->deleteLater();
            m_conn = nullptr;
        }
        if(QMessageBox::critical(nullptr, "critical", "Server error!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
            QApplication* app;
            app->exit(0);
        }
    }

    void MainWindow::onAddFigureReq(const QJsonObject &figure)
    {
        if(m_conn){
            m_conn->addFigure(figure);
        }
    }

    void MainWindow::onDeleteFigureReq(int id)
    {
        if(m_conn){
            m_conn->deleteFigure(id);
        }
    }

    void MainWindow::onClearFiguresReq(int ownerId)
    {
        if(m_conn){
            m_conn->clearFigures(ownerId);
        }
    }


}

