#include "whiteboardconnection.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

namespace wb {

    WBConnection::WBConnection(QObject *parent)
        : QTcpSocket(parent)
    {
        connect(this, SIGNAL(connected()),
                   this, SLOT(onConnected()));
        connect(this, SIGNAL(readyRead()),
                this, SLOT(onReadyRead()));
        connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(onError(QAbstractSocket::SocketError)));
    }

    WBConnection::~WBConnection()
    {}

    void WBConnection::join(QString name, QString host, int port)
    {
        m_name = name;
        connectToHost(host, port);
    }

    void WBConnection::left()
    {
        const char leftMsg[] = "{\"type\":\"left\"}\n";
        write(leftMsg);
    }

    void WBConnection::onConnected()
    {
        qDebug() << __FUNCTION__;
        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("type", QJsonValue("join"));
        obj.insert("name", QJsonValue(m_name));
        doc.setObject(obj);
        QByteArray joinMsg = doc.toJson(QJsonDocument::Compact);
        joinMsg.append('\n');
        write(joinMsg);
    }

    void WBConnection::addFigure(const QJsonObject &figure)
    {
        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("type", QJsonValue("add"));
        obj.insert("figure", QJsonValue(figure));
        doc.setObject(obj);
        QByteArray msg = doc.toJson(QJsonDocument::Compact);
        msg.append('\n');
        write(msg);
        flush();
    }

    void WBConnection::deleteFigure(int id)
    {
        QString msg = QString("{\"type\":\"delete\",\"global_id\":%1}\n").arg(id);
        QByteArray sendMsg = msg.toUtf8();
        write(sendMsg);
        flush();
    }

    void WBConnection::clearFigures(int ownerId)
    {
        QString msg = QString("{\"type\":\"clear\",\"owner_id\":%1}\n").arg(ownerId);
        QByteArray sendMsg = msg.toUtf8();
        write(sendMsg);
        flush();
    }


    void WBConnection::onReadyRead()
    {
        while(canReadLine())
        {
            QByteArray data = readLine();
            data.chop(1);

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(data, &error);
            if(error.error == QJsonParseError::NoError)
            {
                QJsonObject obj = doc.object();
                QString type = obj.value("type").toString();
                if(type == "join_reply")
                {
                    m_id = obj.value("id").toInt();
                    emit joined(m_name, m_id);
                    //extract figures and update
                    QJsonArray figures = obj.value("figures").toArray();
                    for(auto it = figures.begin(); it != figures.end(); it++)
                    {
                        emit figureAdded((*it).toObject());
                    }
                }
                else if(type == "user_joined")
                {
                    QString name = obj.value("name").toString();
                    int id = obj["id"].toInt();
                    emit joined(name, id);
                }
                else if(type == "user_left")
                {
                    QString name = obj.value("name").toString();
                    int id = obj["id"].toInt();
                    emit userLeft(name, id);
                }
                else if(type == "add")
                {
                    emit figureAdded(obj.value("figure").toObject());
                }
                else if(type == "delete")
                {
                    emit figureDeleted(obj.value("global_id").toInt());
                }
                else if(type == "clear")
                {
                    emit figureCleared(obj.value("owner_id").toInt());
                }
                else
                {
                    qDebug() << "WBConnection::onReadyRead, unknown message ,type - " << type;
                }
            }
        }
    }

    void WBConnection::onError(QAbstractSocket::SocketError err)
    {
        qDebug() << "Error occurred " << errorString();
        emit errorOccurred(errorString());
    }
}

