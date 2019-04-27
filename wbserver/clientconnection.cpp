#include "clientconnection.h"
#include <QHostAddress>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

namespace wb{

    int ClientConnection::m_idBase = 0;

    int ClientConnection::generateUserId()
    {
        return ++m_idBase;
    }

    int ClientConnection::m_figureIdBase = 0;
    int ClientConnection::generateFigureId()
    {
        return ++m_figureIdBase;
    }

    ClientConnection::ClientConnection(QObject *parent)
        : QTcpSocket(parent), m_id(-1)
    {
        connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    }

    ClientConnection::~ClientConnection()
    {
        if(m_id > 0)
        {
            emit userLeft(m_name, m_id);
        }
        qDebug() << __FUNCTION__;
    }

    QString ClientConnection::info()
    {
        QString strInfo = QString("%1 : %2").arg(peerAddress().toString())
                .arg(peerPort());
        return strInfo;
    }

    void ClientConnection::onReadyRead()
    {
        while(canReadLine())
        {
            QByteArray data = readLine();
            data.chop(1);
            qDebug() << "msg: " << data.data();
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(data, &error);
            if(error.error == QJsonParseError::NoError)
            {
                QJsonObject rootObj = doc.object();
                QString type = rootObj.value("type").toString();
                if(type == "join")
                {
                    if(m_id < 0)
                    {
                        m_id = generateUserId();
                        QString name = rootObj["name"].toString();
                        m_name = name.toUtf8();
                        qDebug() << name << " join, id - " << m_id << " from " << info();
                        emit userJoined(m_name, m_id);
                    }
                }
                else if(type == "left")
                {
                    if(m_id > 0)
                    {
                        qDebug() << m_name << " left, id - " << m_id << " from " << info();
                        emit userLeft(m_name, m_id);
                    }
                }
                else if(type == "add")
                {
                    QJsonObject figure = rootObj.value("figure").toObject();
                    figure.insert("global_id", QJsonValue(generateFigureId()));
                    emit addFigureReq(figure);
                }
                else if(type == "delete")
                {
                    emit deleteFigureReq(rootObj.value("global_id").toInt());
                }
                else if(type == "clear")
                {
                    emit clearReq(rootObj.value("owner_id").toInt());
                }
                else
                {
                    qDebug() << __FUNCTION__ << " got unknown message ,type - " << type;
                }

            }
            else
            {
                qDebug() << "ClientConnection::onReadyRead, json error - " << error.errorString();
            }
        }
    }

}

