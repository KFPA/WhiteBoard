#include "shapes.h"

int Shape::m_idBase = 0;
int Shape::generateLocalId()
{
    return ++m_idBase;
}

// SLine

SLine::SLine() : Shape(tt_Line), m_rcBounding(0, 0, 0, 0)
{}

void SLine::setStartPoint(const QPointF &pos)
{
    setPos(pos);
    m_startPosScene = pos;
}

void SLine::setEndPoint(const QPointF &pos)
{
    m_endPosScene = pos;
    qreal endX = m_endPosScene.x();
    qreal endY = m_endPosScene.y();
    qreal startX = m_startPosScene.x();
    qreal startY = m_startPosScene.y();

    m_rcBounding.setWidth(qAbs(endX - startX));
    m_rcBounding.setHeight(qAbs(endY - startY));

    QPointF startPoint;
    QPointF endPoint;

    if(endX < startX)
    {
        startX = endX;
        startPoint.setX(m_rcBounding.width());
        endPoint.setX(0);
    }
    else
    {
        startPoint.setX(0);
        endPoint.setX(m_rcBounding.width());
    }

    if(endY < startY)
    {
        startY = endY;
        startPoint.setY(m_rcBounding.height());
        endPoint.setY(0);
    }
    else
    {
        startPoint.setY(0);
        endPoint.setY(m_rcBounding.height());
    }

    m_line.setPoints(startPoint, endPoint);
    setPos(startX, startY);
}

void SLine::setStrokeWidth(float w)
{
    m_strokeWidth = w;
    m_pen.setWidthF(w);
}

void SLine::setStrokeColor(const QColor &clr)
{
    m_strokeColor = clr;
    m_pen.setColor(clr);
}

bool SLine::isValid()
{
    return !m_line.isNull();
}

void SLine::serialize(QJsonObject &obj)
{
    obj.insert("type", QJsonValue("line"));
    QJsonObject data;
    data.insert("color", QJsonValue((qint64)(m_strokeColor.rgba())));
    data.insert("fill_color", QJsonValue((qint64)(m_fillColor.rgba())));
    data.insert("line_width", QJsonValue(m_strokeWidth));
    QJsonArray points;
    points.append(QJsonValue(m_startPosScene.x()));
    points.append(QJsonValue(m_startPosScene.y()));
    points.append(QJsonValue(m_endPosScene.x()));
    points.append(QJsonValue(m_endPosScene.y()));
    data.insert("points", QJsonValue(points));
    obj.insert("data", QJsonValue(data));
}

QRectF SLine::boundingRect() const
{
    return m_rcBounding;
}

void SLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setPen(m_pen);
    painter->drawLine(m_line);
    painter->restore();
}

// SRectangle

SRectangle::SRectangle(int type) : Shape(type), m_rcBounding(0, 0, 0, 0)
{}

void SRectangle::setStartPoint(const QPointF &pos)
{
    setPos(pos);
    m_startPosScene = pos;
}

void SRectangle::setEndPoint(const QPointF &pos)
{
    m_endPosScene = pos;
    qreal endX = m_endPosScene.x();
    qreal endY = m_endPosScene.y();
    qreal startX = m_startPosScene.x();
    qreal startY = m_startPosScene.y();

    m_rcBounding.setWidth(qAbs(endX - startX));
    m_rcBounding.setHeight(qAbs(endY - startY));

    if(endX < startX)
    {
        startX = endX;
    }

    if(endY < startY)
    {
        startY = endY;
    }
    setPos(startX, startY);
}

void SRectangle::setStrokeWidth(float w)
{
    m_strokeWidth = w;
    m_pen.setWidthF(w);
}

void SRectangle::setStrokeColor(const QColor &clr)
{
    m_strokeColor = clr;
    m_pen.setColor(clr);
}

void SRectangle::setFillColor(const QColor &clr)
{
    m_fillColor = clr;
}

bool SRectangle::isValid()
{
    return m_rcBounding.width() >= 1 && m_rcBounding.height() >= 1 ;
}

void SRectangle::serialize(QJsonObject &obj)
{
    switch(m_type)
    {
    case tt_Rectangle:
        obj.insert("type", QJsonValue("rect"));
        break;
    case tt_Oval:
        obj.insert("type", QJsonValue("oval"));
        break;
    case tt_Triangle:
        obj.insert("type", QJsonValue("triangle"));
        break;
    default: return;
    }

    QJsonObject data;
    data.insert("color", QJsonValue((qint64)(m_strokeColor.rgba())));
    data.insert("fill_color", QJsonValue((qint64)(m_fillColor.rgba())));
    data.insert("line_width", QJsonValue(m_strokeWidth));
    QJsonArray points;
    points.append(QJsonValue(m_startPosScene.x()));
    points.append(QJsonValue(m_startPosScene.y()));
    points.append(QJsonValue(m_endPosScene.x()));
    points.append(QJsonValue(m_endPosScene.y()));
    data.insert("points", QJsonValue(points));
    obj.insert("data", QJsonValue(data));
}

QRectF SRectangle::boundingRect() const
{
    return m_rcBounding;
}

void SRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    if(m_strokeWidth > 0.1)
    {
        painter->setPen(m_pen);
    }
    else
    {
        painter->setPen(Qt::NoPen);
    }
    if(m_fillColor == Qt::transparent)
    {
        painter->setBrush(Qt::NoBrush);
    }
    else
    {
        painter->setBrush(m_fillColor);
    }
    painter->drawRect(m_rcBounding);
    painter->restore();
}

// SOval
SOval::SOval() : SRectangle(tt_Oval)
{}

void SOval::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    if(m_strokeWidth > 0.1)
    {
        painter->setPen(m_pen);
    }
    else
    {
        painter->setPen(Qt::NoPen);
    }
    if(m_fillColor == Qt::transparent)
    {
        painter->setBrush(Qt::NoBrush);
    }
    else
    {
        painter->setBrush(m_fillColor);
    }
    painter->drawEllipse(m_rcBounding);
    painter->restore();
}

// STriangle

STriangle::STriangle() : SRectangle(tt_Triangle)
{}

void STriangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    if(m_strokeWidth > 0.1)
    {
        painter->setPen(m_pen);
    }
    else
    {
        painter->setPen(Qt::NoPen);
    }
    if(m_fillColor == Qt::transparent)
    {
        painter->setBrush(Qt::NoBrush);
    }
    else
    {
        painter->setBrush(m_fillColor);
    }

    QPointF points[3] = {
        m_rcBounding.bottomLeft(),
        m_rcBounding.bottomRight(),
        QPointF(m_rcBounding.width()/2, 0)
    };
    painter->drawPolygon(points, 3);

    painter->restore();
}

// SGraffiti
SGraffiti::SGraffiti() : Shape(tt_Graffiti), m_rcBounding(0, 0, 0, 0)
{}

void SGraffiti::setStartPoint(const QPointF &pos)
{
    setPos(pos);
    m_startPosScene = pos;
    m_path.moveTo(pos);
}

void SGraffiti::setEndPoint(const QPointF &pos)
{
    m_path.lineTo(pos);
    m_rcBounding = m_path.boundingRect();
    m_topLeftInScene = m_rcBounding.topLeft();
    setPos(m_topLeftInScene);
    m_rcBounding.moveTo(0, 0);
}

void SGraffiti::setPath(QPainterPath &path)
{
    m_path = path;
    m_rcBounding = m_path.boundingRect();
    m_topLeftInScene = m_rcBounding.topLeft();
    setPos(m_topLeftInScene);
    m_rcBounding.moveTo(0, 0);
}

void SGraffiti::setStrokeWidth(float w)
{
    m_strokeWidth = w;
    m_pen.setWidthF(w);
}

void SGraffiti::setStrokeColor(const QColor &clr)
{
    m_strokeColor = clr;
    m_pen.setColor(clr);
}

bool SGraffiti::isValid()
{
    return !m_path.isEmpty();
}

void SGraffiti::serialize(QJsonObject &obj)
{
    obj.insert("type", QJsonValue("graffiti"));
    QJsonObject data;
    data.insert("color", QJsonValue((qint64)(m_strokeColor.rgba())));
    data.insert("fill_color", QJsonValue((qint64)(m_fillColor.rgba())));
    data.insert("line_width", QJsonValue(m_strokeWidth));
    QJsonArray points;

    int ptCount = m_path.elementCount();
    QPainterPath::Element e;
    for(int i = 0; i < ptCount; i++)
    {
        e = m_path.elementAt(i);
        points.append(QJsonValue(e.x));
        points.append(QJsonValue(e.y));
    }

    data.insert("points", QJsonValue(points));
    obj.insert("data", QJsonValue(data));
}

QRectF SGraffiti::boundingRect() const
{
    return m_rcBounding;
}

void SGraffiti::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setPen(m_pen);
    QPainterPath path = m_path.translated(-m_topLeftInScene);
    painter->drawPath(path);
    painter->restore();
}
