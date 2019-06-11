#include "mainwindow.h"
#include <QtWidgets>
#include <QSizeF>
#include <QImage>
#include <QtDebug>
#include "painterscene.h"
#include "painterview.h"
#include "utils.h"
#include "webbrowser.h"

namespace wb{

    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), m_scene(nullptr)
        , m_conn(nullptr), m_toolBar(nullptr), m_nameEdit(nullptr)
    {
        prepareJoinUI();
    }

    MainWindow::~MainWindow()
    {

    }

    void MainWindow::prepareJoinUI()
    {


        QWidget *widget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(widget);
        QLabel *nameLabel = new QLabel("Input Your Name:");
        layout->addWidget(nameLabel);

        m_nameEdit = new QLineEdit("Panda");
        layout->addWidget(m_nameEdit);

        auto btn = new QPushButton("Join");
        connect(btn, SIGNAL(clicked(bool)),
                this, SLOT(onJoinButtonClicked()));
        layout->addWidget(btn);

        layout->addStretch(1);

        setCentralWidget(widget);
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
            action->setActionGroup(actionGroup);

        }
        else
        {
            addToolBar(m_toolBar);
        }

        m_scene = new PainterScene();
        m_scene->setBkImage(wb::Utils::captureDesk());
        m_scene->setToolType(tt_Graffiti);
        auto *view = new PainterView(m_scene);
        connect(m_scene, SIGNAL(addFigureReq(QJsonObject)),
                this, SLOT(onAddFigureReq(QJsonObject)));
        connect(m_scene, SIGNAL(deleteFigureReq(int)),
                this, SLOT(onDeleteFigureReq(int)));
        connect(m_scene, SIGNAL(clearFiguresReq(int)),
                this, SLOT(onClearFiguresReq(int)));

        setCentralWidget(view);
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
        if(m_scene){
         QString strPath = QDir::currentPath()+"/data/tmp.png";
         m_scene->save(strPath,"PNG");
        }
    }

    void MainWindow::onJoinButtonClicked()
    {
        if(!m_conn)
        {
            m_conn = new WBConnection(this);
            QString strName = m_nameEdit->text();
            connect(m_conn, SIGNAL(joined(QString,int)),
                    this, SLOT(onJoined(QString,int)));
            connect(m_conn, SIGNAL(userLeft(QString,int)),
                    this, SLOT(onUserLeft(QString,int)));
            connect(m_conn, SIGNAL(figureAdded(QJsonObject)),
                    this, SLOT(onFigureAdded(QJsonObject)));
            connect(m_conn, SIGNAL(figureDeleted(int)),
                    this, SLOT(onFigureDeleted(int)));
            connect(m_conn, SIGNAL(figureCleared(int)),
                    this, SLOT(onFiguresCleared(int)));
            m_conn->join(strName, "127.0.0.1", 9001);
        }
    }

    void MainWindow::onJoined(QString name, int id)
    {
        if(id == m_conn->id())
        {
            setWindowTitle(QString("wbclient - %1").arg(m_nameEdit->text()));
            m_nameEdit = nullptr;
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
            prepareJoinUI();
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
            prepareJoinUI();
        }
        if(m_conn)
        {
            m_conn->deleteLater();
            m_conn = nullptr;
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

