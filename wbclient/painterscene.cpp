#include "painterscene.h"
#include <QDebug>

PainterScene::PainterScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_toolType(tt_Line), m_id(-1)
    , m_currentShape(nullptr)
{}

PainterScene::~PainterScene()
{}

void PainterScene::setToolType(int type)
{
    m_toolType = type;
    if(m_currentShape != nullptr)
    {
        if(!m_currentShape->isValid())
        {
            delete m_currentShape;
        }
        m_currentShape = nullptr;
    }
}

void PainterScene::onFigureAdded(const QJsonObject &figure)
{
    qDebug() << "PainterScene::onFigureAdded";
    Shape *item = nullptr;
    QJsonObject data = figure.value("data").toObject();
    QJsonArray points = data.value("points").toArray();
    QString strType = figure.value("type").toString();

    int creatorId = figure.value("creator").toInt();
    int localId = figure.value("local_id").toInt();
    int globalId = figure.value("global_id").toInt();

    if(creatorId == m_id)
    {
        auto it = std::find_if(m_shapes.begin(), m_shapes.end()
                               ,[=](Shape* s){
            return (s->creator() == creatorId && s->localId() == localId);
        });
        if(it != m_shapes.end())
        {
            qDebug() << "udpates figure global id";
            (*it)->setGlobalId(globalId);
            return;
        }
    }

#define SET_START_AND_END_POS() \
    QPointF startPos(points[0].toInt(), points[1].toInt());\
    QPointF endPos(points[2].toInt(), points[3].toInt());\
    item->setStartPoint(startPos);\
    item->setEndPoint(endPos)

    if(strType == "line")
    {
        item = new SLine();
        SET_START_AND_END_POS();
    }
    else if(strType == "rect")
    {
        item = new SRectangle();
        SET_START_AND_END_POS();
    }
    else if(strType == "oval")
    {
        item = new SOval();
        SET_START_AND_END_POS();
    }
    else if(strType == "triangle")
    {
        item = new STriangle();
        SET_START_AND_END_POS();
    }
    else if(strType == "graffiti")
    {
        SGraffiti *graffiti = new SGraffiti();
        item = graffiti;
        QPainterPath path;
        int size = points.size();
        path.moveTo(points[0].toInt(), points[1].toInt());
        for(int i=2; i < size; i += 2)
        {
            path.lineTo(points[i].toInt(), points[i+1].toInt());
        }
        graffiti->setPath(path);
    }
    else
    {
        qDebug() << "unknown figure type!";
        return;
    }

    item->setGlobalId(globalId);
    item->setLocalId(localId);
    item->setStrokeWidth(data.value("line_width").toInt());
    item->setStrokeColor(QColor::fromRgba((unsigned int)data.value("color").toDouble()));
    item->setFillColor(QColor::fromRgba((unsigned int)data.value("fill_color").toDouble()));

    addItem(item);
    m_shapes.push_back(item);
    update();
}

void PainterScene::onFigureDeleted(int id)
{
    auto it = std::find_if(m_shapes.begin(), m_shapes.end(),
                           [=](Shape*s){
        return s->globalId() == id;
    });
    if(it != m_shapes.end())
    {
        qDebug() << "PainterScene::onFigureDeleted, global id - " << id;
        removeItem(*it);
        delete *it;
        m_shapes.erase(it);
        update();
    }
}

void PainterScene::onFiguresCleared(int ownerId)
{
    if(ownerId == -1)
    {
        clear();
        m_shapes.clear();
        update();
        qDebug() << "PainterScene::onFiguresCleared, clear all";
    }
    else
    {
        auto it = m_shapes.begin();
        while(it != m_shapes.end())
        {
            if((*it)->creator() == ownerId)
            {
                removeItem(*it);
                delete *it;
                it = m_shapes.erase(it);
                qDebug() << "PainterScene::onFiguresCleared, clear one figure of " << ownerId;
            }
            else
            {
                it++;
            }
        }
    }
}

void PainterScene::undo()
{
    if(m_shapes.size())
    {
        Shape *item = m_shapes.back();
        emit deleteFigureReq(item->globalId());
    }
}

void PainterScene::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    QGraphicsScene::mousePressEvent(ev);
    if(ev->button() != Qt::LeftButton) return;
    if(!ev->isAccepted())
    {
        switch(m_toolType)
        {
        case tt_Line:
            m_currentShape = new SLine();
            m_currentShape->setStrokeColor(Qt::blue);
            m_currentShape->setStrokeWidth(12);
            break;
        case tt_Rectangle:
            m_currentShape = new SRectangle();
            m_currentShape->setStrokeWidth(8);
            break;
        case tt_Oval:
            m_currentShape = new SOval();
            m_currentShape->setStrokeWidth(0);
            m_currentShape->setFillColor(Qt::red);
            break;
        case tt_Triangle:
            m_currentShape = new STriangle();
            m_currentShape->setStrokeWidth(2);
            m_currentShape->setStrokeColor(QColor::fromRgbF(0.8f, 0.2f, 0.8f));
            break;
        case tt_Graffiti:
            m_currentShape = new SGraffiti();
            m_currentShape->setStrokeWidth(3);
            m_currentShape->setStrokeColor(QColor::fromRgbF(0.9f, 0.1f, 0.6f, 0.7f));
            break;
        default:
            return;
        }

        if(m_currentShape == nullptr) return;
        addItem(m_currentShape);
        m_currentShape->setCreator(m_id);
        m_currentShape->setStartPoint(ev->scenePos());
    }
}

void PainterScene::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    QGraphicsScene::mouseMoveEvent(ev);
    if(!ev->isAccepted() && m_currentShape)
    {
        m_currentShape->setEndPoint(ev->scenePos());
        update();
    }
}

void PainterScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    QGraphicsScene::mouseReleaseEvent(ev);
    if(ev->button() != Qt::LeftButton) return;
    if(!ev->isAccepted() && m_currentShape)
    {
        if(m_currentShape->isValid())
        {
            m_shapes.push_back(m_currentShape);
            QJsonObject figure;
            m_currentShape->serialize(figure);
            figure.insert("creator", QJsonValue(m_id));
            figure.insert("local_id", QJsonValue(m_currentShape->localId()));
            emit addFigureReq(figure);
        }
        else
        {
            removeItem(m_currentShape);
            delete m_currentShape;
        }
        m_currentShape = nullptr;
    }
}
