#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "painterscene.h"
#include "whiteboardconnection.h"
#include "utils.h"

namespace wb{

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
    public:
        void setInfo(WBInfo wbInfo){m_wbInfo = wbInfo;}

        bool Join();
    protected slots:
        void onDrawLineAction();
        void onDrawRectangleAction();
        void onDrawOvalAction();
        void onDrawTriangleAction();
        void onDrawGraffitiAction();
        void onUndo();
        void onClearAll();
        void onSave();
        void onClose();



        void onJoined(QString name, int id);
        void onUserLeft(QString name, int id);
        void onFigureAdded(const QJsonObject &figure);
        void onFigureDeleted(int id);
        void onFiguresCleared(int ownerId);
        void onErrorOccurred(const QString & desc);

        void onAddFigureReq(const QJsonObject &figure);
        void onDeleteFigureReq(int id);
        void onClearFiguresReq(int ownerId);

    protected:
        void prepareJoinUI();
        void preparePainterUI();
        void closeEvent(QCloseEvent *event);

    protected:
        PainterScene *m_scene;
        WBConnection *m_conn;
        QToolBar *m_toolBar;
        WBInfo   m_wbInfo;
    };



}

#endif // MAINWINDOW_H
