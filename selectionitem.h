#ifndef SELECTIONITEM_H
#define SELECTIONITEM_H

#include <QGraphicsItem>
#include "triggeritem.h"

class QGraphicsItem;
class TriggerItem;

class SelectionItem : public QGraphicsItem
{
    //Q_OBJECT

public:
//    explicit SelectionItem(qreal x1, qreal y1, qreal x2, qreal y2,
//                           QGraphicsItem *parent = 0);
    explicit SelectionItem(TriggerItem *leftSelected, TriggerItem *rightSelected,
                           qreal y1, qreal y2, QGraphicsItem *parent = 0);
    enum { Type = UserType + 2 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void transmitMouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void moveOneStepLeft();
    void moveOneStepRight();

private:
    QPolygonF polygon;
    QGraphicsTextItem *text;
    QGraphicsTextItem *textBefore;
    QGraphicsTextItem *textAfter;
    QGraphicsLineItem *lineBefore, *lineBeforeLeft, *lineBeforeRight;
    QGraphicsLineItem *lineAfter, *lineAfterLeft, *lineAfterRight;

    TriggerItem *leftSelected;
    TriggerItem *rightSelected;
    TriggerItem *before;
    TriggerItem *after;

    void updateDeltaLines();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:

public slots:

};

#endif // SELECTIONITEM_H
