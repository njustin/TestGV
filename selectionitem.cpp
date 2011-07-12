#include <QtGui>

#include "selectionitem.h"
#include "timelinescene.h"

/*SelectionItem::SelectionItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    //qDebug() << "new x1" << x1 << "y1" << y1 << "x2" << x2 << "y2" << y2;
    this->polygon << QPointF(x1, y1) << QPointF(x1, y2)
            << QPointF(x2, y2) << QPointF(x2, y1);

    text = new QGraphicsTextItem(this);
    text->setPlainText(QString::number(x1));
    text->setPos(x1, 90);

    this->setFlags(ItemIsMovable);
}*/

SelectionItem::SelectionItem(TriggerItem *leftSelected, TriggerItem *rightSelected,
                             qreal y1, qreal y2, QGraphicsItem *parent) : QGraphicsItem(parent)
{
    this->leftSelected = leftSelected;
    this->rightSelected = rightSelected;

    TimelineScene *timeline = qobject_cast<TimelineScene *>(this->leftSelected->scene());
    this->before = timeline->getTriggerItemBefore(leftSelected);
    this->after = timeline->getTriggerItemAfter(rightSelected);

    textBefore = NULL; lineBefore = NULL; lineBeforeLeft = NULL; lineBeforeRight = NULL;
    textAfter = NULL; lineAfter = NULL; lineAfterLeft = NULL; lineAfterRight = NULL;

    qreal x1, x2;
    x1 = this->leftSelected->x();
    x2 = this->rightSelected->sceneBoundingRect().bottomRight().x();
    this->polygon << QPointF(x1, y1) << QPointF(x1, y2)
            << QPointF(x2, y2) << QPointF(x2, y1);

    // Text label for current position
    //text = new QGraphicsTextItem(this);
    //text->setHtml("<b>"+this->leftSelected->label()+"</b>" + "<br>" + QString::number(x1 * timeline->ratio(), 'f', 1));
    //text->setPlainText(QString::number(x1 * timeline->ratio(), 'f', 1));
    //text->setPos(x1, this->leftSelected->sceneBoundingRect().top() - text->sceneBoundingRect().height());
    //text->setPos((x1 - 15.0) - text->sceneBoundingRect().width(),
    //             -20.0 - text->sceneBoundingRect().height());

    // Position indicator
    //QGraphicsLineItem* l1 = new QGraphicsLineItem(QLineF(x1, 0.0, x1-15.0, -20.0), this);
    //QGraphicsLineItem* l2 = new QGraphicsLineItem(QLineF(x1-15.0, -20.0,
    //                                                     (x1 - 15.0) - text->sceneBoundingRect().width(), -20.0 ),
    //                                              this);

    this->setFlags(ItemIsMovable);
    updateDeltaLines();
}

#if 0
void SelectionItem::updateDeltaLines()
{
    qreal x1, x2;
    x1 = this->leftSelected->x();
    x2 = this->rightSelected->sceneBoundingRect().bottomRight().x();

    // Text label for the delta between the left selected and the earlier item
    if (this->before) {
        if (textBefore == NULL && lineBefore == NULL) {
            // Create the elements
            textBefore = new QGraphicsTextItem(this);
            textBefore->setPlainText(QString::number(this->leftSelected->x() - this->before->pos().x()));
            textBefore->setPos(this->mapFromScene(x1 - textBefore->sceneBoundingRect().width(), 30.0));

            lineBefore = new QGraphicsLineItem(this);
            //qDebug() << "new lineBefore "<<this->leftSelected->x()<<this->before->pos().x();
            lineBefore->setLine(QLineF(this->mapFromScene(this->leftSelected->x(), 30.0),
                                       this->mapFromScene(this->before->pos().x(), 30.0)));
        }
        else {
            // Update the line and text label
            qreal d = this->leftSelected->x() - this->before->pos().x();
            //if (d < 0) {
                // We have crossed one item, update the atfer and before item
                TimelineScene *timeline = qobject_cast<TimelineScene *>(this->leftSelected->scene());
                this->before = timeline->getTriggerItemBefore(this->leftSelected);
                this->after = timeline->getTriggerItemAfter(this->rightSelected);

                if (this->before) {
                    // Update the before line
                    d = this->leftSelected->x() - this->before->pos().x();
                    QLineF line = this->lineBefore->line();
                    QPointF p2 = line.p2();
                    p2.setX(this->mapFromScene(this->before->pos()).x());
                    line.setP2(p2);
                    line.setLength(d);
                    this->lineBefore->setLine(line);
                    textBefore->setPlainText(QString::number(d));
                } else {
                    // No more before item, remove everything
                    qDebug() << "Remove before item";
                    timeline->removeItem(this->lineBefore);
                    delete this->lineBefore;
                    this->lineBefore = NULL;
                    timeline->removeItem(this->textBefore);
                    delete this->textBefore;
                    this->textBefore = NULL;
                }
                // the after line will be handled in the next part of this method
            /*} else {
                textBefore->setPlainText(QString::number(d));
                QLineF line = this->lineBefore->line();
                line.setLength(d);
                this->lineBefore->setLine(line);
            }*/
        }
    }

    // Text label for the delta between the right selected and the later item
    if (this->after) {
        if (textAfter == NULL && lineAfter == NULL) {
            // Create
            textAfter = new QGraphicsTextItem(this);
            textAfter->setPlainText(QString::number(this->after->pos().x() - this->rightSelected->x()));
            textAfter->setPos(this->mapFromScene(x2, 40.0));

            lineAfter = new QGraphicsLineItem(this);
            lineAfter->setLine(QLineF(this->mapFromScene(this->rightSelected->x(), 40.0),
                                      this->mapFromScene(this->after->pos().x(), 40.0)));
            qDebug() << "Create lineAfter" << QLineF(this->mapFromScene(this->rightSelected->x(), 40.0),
                                                     this->mapFromScene(this->after->pos().x(), 40.0));
        }
        else {
            // Update
            qreal d = this->after->pos().x() - this->rightSelected->x();
            //if (d < 0) {
                // We have crossed one item, update the atfer and before item
                TimelineScene *timeline = qobject_cast<TimelineScene *>(this->leftSelected->scene());
                this->before = timeline->getTriggerItemBefore(this->leftSelected);
                this->after = timeline->getTriggerItemAfter(this->rightSelected);

                if (this->after) {
                    // Update the after line
                    d = this->after->pos().x() - this->rightSelected->x();
                    QLineF line = this->lineAfter->line();
                    QPointF p2 = line.p2();
                    p2.setX(this->mapFromScene(this->after->pos()).x());
                    line.setP2(p2);
                    line.setLength(d);
                    this->lineAfter->setLine(line);
                    textAfter->setPlainText(QString::number(d));
                    qDebug() << "Update lineAfter" << line;
                } else {
                    // No more after item, remove everything
                    qDebug() << "Remove after item";
                    timeline->removeItem(this->lineAfter);
                    delete this->lineAfter;
                    this->lineAfter = NULL;
                    timeline->removeItem(this->textAfter);
                    delete this->textAfter;
                    this->textAfter = NULL;
                }
            /*} else {
                textAfter->setPlainText(QString::number(d));
                QLineF line = this->lineAfter->line();
                line.setLength(d);
                this->lineAfter->setLine(line);
            }*/
        }
    }
}
#endif

void SelectionItem::updateDeltaLines()
{
    qreal x1, x2;
    x1 = this->leftSelected->x();
    x2 = this->rightSelected->sceneBoundingRect().bottomRight().x();

    // Update before and after items since we may have crossed over an item
    TimelineScene *timeline = qobject_cast<TimelineScene *>(this->leftSelected->scene());
    this->before = timeline->getTriggerItemBefore(this->leftSelected);
    this->after = timeline->getTriggerItemAfter(this->rightSelected);

    if (this->before) {
        qreal d = this->leftSelected->x() - this->before->pos().x();
        if (textBefore == NULL && lineBefore == NULL) {
            // Create the elements
            QPointF p1 = this->mapFromScene(this->leftSelected->x(), 30.0);
            QPointF p2 = this->mapFromScene(this->before->pos().x(), 30.0);

            textBefore = new QGraphicsTextItem(this);
            textBefore->setPlainText(QString::number(d * timeline->ratio(), 'f', 1));
            //textBefore->setPos(this->mapFromScene(x1 - textBefore->sceneBoundingRect().width(), 30.0));
            textBefore->setPos( (p1.x() + p2.x()) / 2, p1.y());
            lineBefore = new QGraphicsLineItem(this);
            lineBefore->setLine(QLineF(p1, p2));
            lineBeforeRight = new QGraphicsLineItem(p1.x(), 0.0, p1.x(), 40.0, this);
            lineBeforeLeft = new QGraphicsLineItem(p2.x(), 0.0, p2.x(), 40.0, this);
        }
        else {
            // Update the before line and label
            QLineF line = this->lineBefore->line();
            QPointF p2 = line.p2();
            p2.setX(this->mapFromScene(this->before->pos()).x());
            line.setP2(p2);
            line.setLength(d);
            this->lineBefore->setLine(line);
            textBefore->setPos((line.p1().x() + line.p2().x())/2,
                               line.p1().y());
            textBefore->setPlainText(QString::number(d * timeline->ratio(), 'f', 1));

            // the left before line should not move
            this->lineBeforeLeft->setLine(QLine(this->mapFromScene(this->before->pos()).x(),
                                                this->lineBeforeLeft->line().y1(),
                                                this->mapFromScene(this->before->pos()).x(),
                                                this->lineBeforeLeft->line().y2()));
        }
    } else {
        // No more before item, remove everything
        if (this->lineBefore) {
            timeline->removeItem(this->lineBefore);
            timeline->removeItem(this->lineBeforeLeft);
            timeline->removeItem(this->lineBeforeRight);
            delete this->lineBefore;
            delete this->lineBeforeLeft;
            delete this->lineBeforeRight;
            this->lineBefore = NULL; this->lineBeforeLeft = NULL; this->lineBeforeRight = NULL;
        }
        if (this->textBefore) {
            timeline->removeItem(this->textBefore);
            delete this->textBefore;
            this->textBefore = NULL;
        }
    }

    // Text label for the delta between the right selected and the later item
    if (this->after) {
        qreal d = this->after->pos().x() - this->rightSelected->x();
        if (textAfter == NULL && lineAfter == NULL) {
            // Create
            QPointF p1 = this->mapFromScene(this->rightSelected->x(), 30.0);
            QPointF p2 = this->mapFromScene(this->after->pos().x(), 30.0);
            textAfter = new QGraphicsTextItem(this);
            textAfter->setPlainText(QString::number(d * timeline->ratio(), 'f', 1));
            textAfter->setPos(this->mapFromScene(x2, 30.0));

            lineAfter = new QGraphicsLineItem(this);
            lineAfter->setLine(QLineF(p1, p2));
            lineAfterLeft = new QGraphicsLineItem(p1.x(), 0.0, p1.x(), 40.0, this);
            lineAfterRight = new QGraphicsLineItem(p2.x(), 0.0, p2.x(), 40.0, this);
        }
        else {
            // Update the after line and label
            QLineF line = this->lineAfter->line();
            QPointF p = line.p2();
            p.setX(this->mapFromScene(this->after->pos()).x());
            line.setP2(p);
            line.setLength(d);
            this->lineAfter->setLine(line);
            textAfter->setPlainText(QString::number(d * timeline->ratio(), 'f', 1));

            // the right after line should not move
            this->lineAfterRight->setLine(QLine(this->mapFromScene(this->after->pos()).x(),
                                                this->lineAfterRight->line().y1(),
                                                this->mapFromScene(this->after->pos()).x(),
                                                this->lineAfterRight->line().y2()));
        }
    } else {
        // No more after item, remove everything
        if (this->lineAfter) {
            timeline->removeItem(this->lineAfter);
            timeline->removeItem(this->lineAfterLeft);
            timeline->removeItem(this->lineAfterRight);
            delete this->lineAfter;
            delete this->lineAfterLeft;
            delete this->lineAfterRight;
            this->lineAfter = NULL; this->lineAfterLeft = NULL; this->lineAfterRight = NULL;
        }
        if (this->textAfter) {
            timeline->removeItem(this->textAfter);
            delete this->textAfter;
            this->textAfter = NULL;
        }
    }
}

QRectF SelectionItem::boundingRect() const
{
    return this->polygon.boundingRect();
}

QPainterPath SelectionItem::shape() const
{
    QPainterPath path;
    path.addPolygon(this->polygon);
    return path;
}

void SelectionItem::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::lightGray);
    painter->setOpacity(0.2);

    QPen myPen;
    myPen.setJoinStyle(Qt::MiterJoin);
    myPen.setWidthF(1.0);
    myPen.setColor(Qt::black);
    painter->setPen(myPen);

    painter->drawPolygon(this->polygon);
}

// Go through the 'protected' barrier
void SelectionItem::transmitMouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->mouseMoveEvent(event);
}

void SelectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "SelectionItem::MouseMoveEvent" << event->scenePos() << this->leftSelected->scenePos();

    // TODO: Keep the selection in a valid timerange
    //if (this->leftSelected->x() <= 0.0) return;

    QGraphicsItem::mouseMoveEvent(event);
    this->setPos(this->x() , 0.0);

    QList<QGraphicsItem *> selectedItems;
    selectedItems = this->scene()->selectedItems();
    foreach (QGraphicsItem *item, selectedItems) {
        item->setPos(item->x(), 0.0);
    }

    //TimelineScene *timeline = qobject_cast<TimelineScene *>(this->scene());
    //this->text->setPlainText(QString::number(this->leftSelected->x() * timeline->ratio(), 'f', 1));
    /*this->text->setHtml("<b>"+this->leftSelected->label()+"</b>" + "<br>"
                        + QString::number(this->leftSelected->x() * timeline->ratio(), 'f', 1));*/
    updateDeltaLines();
}

void SelectionItem::moveOneStepLeft()
{
    if (this->leftSelected->x() <= 0.0) return;
    TimelineScene *timeline = qobject_cast<TimelineScene *>(this->scene());

    this->moveBy(-(1/timeline->ratio()), 0.0);
    QList<QGraphicsItem *> selectedItems;
    selectedItems = this->scene()->selectedItems();
    foreach (QGraphicsItem *item, selectedItems) {
        item->moveBy(-(1/timeline->ratio()), 0.0);
    }
    //this->text->setPlainText(QString::number(this->leftSelected->x() * timeline->ratio(), 'f', 1));
    /*this->text->setHtml("<b>"+this->leftSelected->label()+"</b>" + "<br>"
                        + QString::number(this->leftSelected->x() * timeline->ratio(), 'f', 1));*/
    updateDeltaLines();
}

void SelectionItem::moveOneStepRight()
{
    TimelineScene *timeline = qobject_cast<TimelineScene *>(this->scene());

    if (this->rightSelected->x() >= timeline->maxLength() * (1/timeline->ratio())) return;

    this->moveBy(1/timeline->ratio(), 0.0);
    QList<QGraphicsItem *> selectedItems;
    selectedItems = this->scene()->selectedItems();
    foreach (QGraphicsItem *item, selectedItems) {
        item->moveBy(1/timeline->ratio(), 0.0);
    }
    //this->text->setPlainText(QString::number(this->leftSelected->x() * timeline->ratio(), 'f', 1));
    /*this->text->setHtml("<b>"+this->leftSelected->label()+"</b>" + "<br>"
                        + QString::number(this->leftSelected->x() * timeline->ratio(), 'f', 1));*/
    updateDeltaLines();
}
