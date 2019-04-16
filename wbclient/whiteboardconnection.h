#ifndef WHITEBOARDCONNECTION_H
#define WHITEBOARDCONNECTION_H

#include <QTcpSocket>

class WBConnection : public QTcpSocket
{
    Q_OBJECT
public:
    WBConnection(QObject *parent = 0);
    ~WBConnection();

    void join(QString name, QString host, int port);
    void left();

    int id(){ return m_id; }

    void addFigure(const QJsonObject &figure);
    void deleteFigure(int id);
    void clearFigures(int ownerId);

signals:
    void joined(QString name, int id);
    void userLeft(QString name, int id);
    void figureAdded(const QJsonObject &figure);
    void figureDeleted(int id);
    void figureCleared(int ownerId);
    void errorOccurred(const QString & desc);

protected slots:
    void onConnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError err);

protected:
    QString m_name;
    int m_id;
};

#endif // WHITEBOARDCONNECTION_H
