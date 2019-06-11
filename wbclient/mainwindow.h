#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "painterscene.h"
#include "whiteboardconnection.h"

namespace wb{

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    protected slots:
        void onDrawLineAction();
        void onDrawRectangleAction();
        void onDrawOvalAction();
        void onDrawTriangleAction();
        void onDrawGraffitiAction();
        void onUndo();
        void onClearAll();
        void onSave();

        void onJoinButtonClicked();

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

    protected:
        PainterScene *m_scene;
        WBConnection *m_conn;
        QLineEdit *m_nameEdit;
        QToolBar *m_toolBar;
    };

}

#endif // MAINWINDOW_H
