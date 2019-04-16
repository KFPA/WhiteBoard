#ifndef PAINTERSCENE_H
#define PAINTERSCENE_H

#include <QGraphicsScene>
#include "shapes.h"
#include <QJsonObject>
#include <list>

class PainterScene : public QGraphicsScene
{
    Q_OBJECT
public:
    PainterScene(QObject *parent = 0);
    ~PainterScene();

    void setToolType(int type);

    void setUserId(int id){ m_id = id; }

    void onFigureAdded(const QJsonObject &figure);
    void onFigureDeleted(int id);
    void onFiguresCleared(int ownerId);
    void undo();

signals:
    void addFigureReq(const QJsonObject &figure);
    void deleteFigureReq(int id);
    void clearFiguresReq(int ownerId);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);

protected:
    int m_toolType;
    int m_id;
    Shape *m_currentShape;
    std::list<Shape*> m_shapes;
};

#endif // PAINTERSCENE_H
