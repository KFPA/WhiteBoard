#ifndef SHAPES_H
#define SHAPES_H

#include <QtWidgets>

enum ToolType {
    tt_Line = 1,
    tt_Graffiti,
    tt_Rectangle,
    tt_Oval,
    tt_Triangle
};

class Shape : public QGraphicsItem
{
public:
    Shape(int type): m_type(type)
      , m_strokeWidth(1.0f)
      , m_strokeColor(Qt::black)
      , m_fillColor(Qt::transparent)
      , m_creatorId(-1), m_globalId(-1)
      , m_localId(generateLocalId())
    {}

    virtual ~Shape(){}

    virtual void setStartPoint(const QPointF &pos) = 0;
    virtual void setEndPoint(const QPointF &pos) = 0;
    virtual void setStrokeWidth(float w) { m_strokeWidth = w; }
    virtual void setStrokeColor(const QColor &clr){ m_strokeColor = clr; }
    virtual void setFillColor(const QColor &clr){ m_fillColor = clr; }
    virtual bool isValid(){ return true; }
    virtual void serialize(QJsonObject &obj) = 0;

    static int generateLocalId();

    void setCreator(int id){ m_creatorId = id;}
    int creator(){ return m_creatorId; }
    void setGlobalId(int id) { m_globalId = id; }
    int globalId(){ return m_globalId; }
    void setLocalId(int id){ m_localId = id; }
    int localId(){ return m_localId; }

protected:
    int m_type;
    float m_strokeWidth;
    QColor m_strokeColor;
    QColor m_fillColor;
    static int m_idBase;
    int m_creatorId;
    int m_globalId;
    int m_localId;
};

class SLine : public Shape
{
public:
    SLine();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setStartPoint(const QPointF &pos) override;
    void setEndPoint(const QPointF &pos) override;
    void setStrokeWidth(float w) override;
    void setStrokeColor(const QColor &clr) override;
    bool isValid() override;
    void serialize(QJsonObject &obj) override;

protected:
    QPointF m_startPosScene;
    QPointF m_endPosScene;
    QLineF m_line;
    QRectF m_rcBounding;
    QPen m_pen;
};

class SRectangle : public Shape
{
public:
    SRectangle(int type = tt_Rectangle);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setStartPoint(const QPointF &pos) override;
    void setEndPoint(const QPointF &pos) override;
    void setStrokeWidth(float w) override;
    void setStrokeColor(const QColor &clr) override;
    void setFillColor(const QColor &clr) override;
    bool isValid() override;
    void serialize(QJsonObject &obj) override;

protected:
    QPointF m_startPosScene;
    QPointF m_endPosScene;
    QRectF m_rcBounding;
    QPen m_pen;
};

class SOval : public SRectangle
{
public:
    SOval();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class STriangle : public SRectangle
{
public:
    STriangle();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class SGraffiti : public Shape
{
public:
    SGraffiti();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setStartPoint(const QPointF &pos) override;
    void setEndPoint(const QPointF &pos) override;
    void setStrokeWidth(float w) override;
    void setStrokeColor(const QColor &clr) override;
    bool isValid() override;
    void serialize(QJsonObject &obj) override;
    void setPath(QPainterPath &path);

protected:
    QPointF m_startPosScene;
    QPointF m_endPosScene;
    QRectF m_rcBounding;
    QPen m_pen;
    QPainterPath m_path;
    QPointF m_topLeftInScene;
};


#endif // SHAPES_H
