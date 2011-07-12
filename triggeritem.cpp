#include <QtGui>
#include <QPainter>

#include "triggeritem.h"
#include "selectionitem.h"
#include "timelinescene.h"

TriggerItem::TriggerItem(int address, int sequence, QString label, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    mAddress = address;
    mLabel = label;
    mSequence = sequence;
    this->polygon << QPointF(0.0, -10.0) << QPointF(30.0, 0.0) << QPointF(0.0, 10.0);
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges | ItemIsFocusable);

    //
    textItem = new QGraphicsTextItem(this);
    l1 = new QGraphicsLineItem(this);
    l2 = new QGraphicsLineItem(this);
}

QRectF TriggerItem::boundingRect() const
{
    return QRectF(0.0, -10.0, 30.0, 20.0);
}

QPainterPath TriggerItem::shape() const
{
    QPainterPath path;
    path.addPolygon(this->polygon);
    return path;
}

void TriggerItem::paint(QPainter *painter,
           const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    //Q_UNUSED(option);
    Q_UNUSED(widget);
    if (option->state & QStyle::State_Selected) {
        //qDebug() << "Selected";
    }
    QLinearGradient myGradient;
    myGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    if (this->isSelected()) {
        myGradient.setColorAt(0, Qt::green);
    } else {
        myGradient.setColorAt(0, Qt::blue);
    }
    myGradient.setColorAt(1, Qt::white);
    painter->setRenderHint(QPainter::Antialiasing);
    //painter->setBrush(Qt::blue);
    painter->setBrush(myGradient);

    /*QPen myPen;
    myPen.setJoinStyle(Qt::RoundJoin);
    myPen.setWidthF(0.0);
    myPen.setColor(Qt::blue);
    painter->setPen(myPen);*/
    painter->setPen(Qt::white);

    //painter->drawRoundedRect(0, 0, 250, 250, 5, 5);
    painter->drawPolygon(this->polygon);
}

void TriggerItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    qDebug() << "TriggerItem::mouseDoubleClickEvent()";
}

void TriggerItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    qDebug() << "TriggerItem::contextMenuEvent";
}

void TriggerItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //if (this->x() <= 0.0) return;

//    QGraphicsItem::mouseMoveEvent(event);

    QGraphicsItem* selection = this->scene()->itemAt(event->scenePos().x(), event->scenePos().y());
    if (selection != NULL && selection->type() == SelectionItem::Type) {
        // Let the SelectionItem::mouseMoveEvent handle all moving
        SelectionItem* s = qgraphicsitem_cast<SelectionItem *>(selection);
        s->transmitMouseMoveEvent(event);
    }
}

QVariant TriggerItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //qDebug() << change;
    if (change == ItemPositionHasChanged) {
        //qDebug() << "ItemPositionHasChanged" << this->label();
    }
    if (change == QGraphicsItem::ItemPositionChange) {
        //qDebug() << "ItemPositionChange" << this->label();
        // Text item for label, current position and address
        qreal x1 = 0.0;
        TimelineScene *timeline = qobject_cast<TimelineScene *>(this->scene());
        textItem->setHtml("<b>"+this->label()+"</b> (" +QString::number(this->address())+ ")<br>"
                      + QString::number(value.toPointF().x() * timeline->ratio(), 'f', 1));
        textItem->setPos((x1 - 15.0) - textItem->sceneBoundingRect().width(),
                     -20.0 - textItem->sceneBoundingRect().height());
        l1->setLine(QLineF(x1, 0.0, x1-15.0, -20.0));
        l2->setLine(QLineF(x1-15.0, -20.0, (x1 - 15.0) - textItem->sceneBoundingRect().width(), -20.0 ));
    }
    return QGraphicsItem::itemChange(change, value);
}

void TriggerItem::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "TriggerItem::keyPressEvent";
    if (event->key() == Qt::Key_Delete || event == QKeySequence::Delete) {
        TimelineScene *timeline = qobject_cast<TimelineScene *>(this->scene());
        QList<QGraphicsItem *> selectedItems;
        selectedItems = this->scene()->selectedItems();
        foreach (QGraphicsItem *item, selectedItems) {
            if (item->type() == TriggerItem::Type) {
                TriggerItem *triggerItem = qgraphicsitem_cast<TriggerItem *>(item);
                timeline->removeTriggerItem(triggerItem);
            }
        }
    }
    else if (event->key() == Qt::Key_Right || event == QKeySequence::MoveToNextChar) {
        TimelineScene *timeline = qobject_cast<TimelineScene *>(this->scene());
        if (timeline->selectionItem != NULL) {
            timeline->selectionItem->moveOneStepRight();
        }
    } else if (event->key() == Qt::Key_Left || event == QKeySequence::MoveToPreviousChar) {
        TimelineScene *timeline = qobject_cast<TimelineScene *>(this->scene());
        if (timeline->selectionItem != NULL) {
            timeline->selectionItem->moveOneStepLeft();
        }
    }
}
