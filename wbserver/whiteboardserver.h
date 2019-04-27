#ifndef WHITEBOARDSERVER_H
#define WHITEBOARDSERVER_H

#include <QTcpServer>
#include <list>
#include "clientconnection.h"
#include <QJsonArray>

namespace wb {

    class WhiteboardServer : public QTcpServer
    {
        Q_OBJECT
    public:
        WhiteboardServer(QObject *parent = 0);
        ~WhiteboardServer();

    protected:
        void incomingConnection(qintptr sock);

    protected slots:
        void onUserJoined(QByteArray name, int id);
        void onUserLeft(QByteArray name, int id);
        void onAddFigureReq(const QJsonObject &figure);
        void onDeleteFigureReq(int globalId);
        void onClearReq(int ownerId);

    protected:
        std::list<ClientConnection*> m_clients;
        QJsonArray m_figures;
    };


}

#endif // WHITEBOARDSERVER_H
