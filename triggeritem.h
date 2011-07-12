#ifndef TRIGGERITEM_H
#define TRIGGERITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#define MIN_ADDRESS 0
#define MAX_ADDRESS 256

class QGraphicsItem;

class TriggerItem : public QGraphicsItem
{

public:
    explicit TriggerItem(int address, int sequence, QString label, QGraphicsItem *parent = 0);

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    int address() { return mAddress; }
    int sequence() { return mSequence; }
    QString label() { return mLabel; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
private:
    QPolygonF polygon;
    int mAddress;
    int mSequence;
    QString mLabel;

    QGraphicsTextItem *textItem;
    QGraphicsLineItem *l1, *l2;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:

public slots:

};

#endif // TRIGGERITEM_H
