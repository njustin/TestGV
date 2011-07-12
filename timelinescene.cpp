#include <QtGui>

#include "timelinescene.h"
#include "selectionitem.h"

TimelineScene::TimelineScene(QObject *parent) :
    QGraphicsScene(parent)
{
    mRatio = 0.1;
    mMaxLength = 3600.0;

    // Draw the time line
    QGraphicsItem *timeLine;
    timeLine = this->addLine(0.0, 0.0,
                             mMaxLength * (1/mRatio), 0.0);
    QTime timeMark(0, 0, 0);
    int j = 0;
    for (qreal i = 0; i <= mMaxLength * (1/mRatio);
         i = i + (1/mRatio), j++, timeMark = timeMark.addSecs(1)) {
        if (j == 0 || j == 5 || j == 10) {
            this->addLine(i, 0.0, i, -5.0);
        } else {
            this->addLine(i, 0.0, i, -2.0);
        }
        if (j == 0 || j == 10) {
            // Display a time mark evey tenth seconds
            if (timeMark.hour() == 0) {
                this->addText(timeMark.toString("mm:ss"))->moveBy(i, -2.0);
            } else {
                this->addText(timeMark.toString("h:mm:ss"))->moveBy(i, -2.0);
            }
            j = 0;
        }
    }

    leftSelectedItem = NULL;
    rightSelectedItem = NULL;
    selectionItem = NULL;

    connect(this, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
}

qreal TimelineScene::ratio()
{
    return mRatio;
}

qreal TimelineScene::maxLength()
{
    return mMaxLength;
}

bool TimelineScene::addTriggerItem(qreal x, int address, int sequence, QString label)
{
    if (address < MIN_ADDRESS || address > MAX_ADDRESS
        || sequence < MIN_SEQUENCE || sequence > MAX_SEQUENCE ) return false;

    // Check for address unicity, return false if the address already exists
    foreach (TriggerItem *it, this->triggerItems) {
        if (it->address() == address) return false;
    }

    TriggerItem *item = new TriggerItem(address, sequence, label);
    this->triggerItems.push_back(item);
    this->sequences[sequence].push_back(item);
    this->addItem(item);

    // Move the y position elsewhere than y=0.0, otherwise the ItemPositionChange event will not
    // be triggered if x == 0.0
    item->setPos(x * (1/mRatio), 0.0000000001);
    return true;
}

void TimelineScene::removeTriggerItem(TriggerItem *item)
{
    if (item == NULL) return;
    foreach (TriggerItem *it, this->triggerItems) {
        if (item == it) {
            this->triggerItems.removeOne(it);
            this->removeItem(it);
            delete it;
        }
    }
    this->sequences[item->sequence()].removeOne(item);
}

TriggerItem* TimelineScene::getTriggerItemBefore(TriggerItem *item)
{
    TriggerItem *ret = NULL;
    foreach (TriggerItem* it, this->triggerItems) {
        if (it->x() < item->x()
            && (ret == NULL || it->x() >= ret->x())) {
            ret = it;
        }
    }
    return ret;
}

TriggerItem* TimelineScene::getTriggerItemAfter(TriggerItem *item)
{
    TriggerItem *ret = NULL;
    foreach (TriggerItem* it, this->triggerItems) {
        if (it->x() > item->x()
            && (ret == NULL || it->x() <= ret->x())) {
            ret = it;
        }
    }
    return ret;
}

void TimelineScene::drawSelection()
{
    if (selectionItem != NULL) {
        // Always recreate the SelectionItem, avoid some redraw bugs
        //this->removeItem(selectionItem);
        delete selectionItem;
        selectionItem = NULL;
    }

    if (leftSelectedItem == NULL || rightSelectedItem == NULL) return;

    QRectF sceneRect = (this->views().first()->mapToScene(this->views().first()->contentsRect())).boundingRect();

    qreal x1, x2, y1, y2;
    x1 = leftSelectedItem->x();
    x2 = rightSelectedItem->sceneBoundingRect().bottomRight().x();
    y1 = sceneRect.y() - 20.0;//this->views().first()->contentsRect().topLeft().y();
    y2 = sceneRect.y() + sceneRect.height() + 20.0; //this->views().first()->contentsRect().bottomRight().y();

    selectionItem = new SelectionItem(leftSelectedItem, rightSelectedItem, y1, y2);
    this->addItem(selectionItem);
}

void TimelineScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setFocusItem(leftSelectedItem);
    QGraphicsScene::mouseMoveEvent(event);
}

void TimelineScene::updateSelection()
{
    QList<QGraphicsItem*> selectedItems = this->selectedItems();

    // Reset the left and right selectedItem, if one of them has been removed
    leftSelectedItem = NULL;
    rightSelectedItem = NULL;

    if (selectedItems.isEmpty()) {
        leftSelectedItem = NULL;
        rightSelectedItem = NULL;
    }
    else if (selectedItems.size() == 1) {
        if (selectedItems.first()->type() != TriggerItem::Type) {
            leftSelectedItem = NULL;
            rightSelectedItem = NULL;
        } else {
            leftSelectedItem = qgraphicsitem_cast<TriggerItem *>(selectedItems.first());
            rightSelectedItem = qgraphicsitem_cast<TriggerItem *>(selectedItems.first());
        }
    }
    else {
        foreach (QGraphicsItem *item, selectedItems) {
            if (item->type() == TriggerItem::Type) {
                if (leftSelectedItem == NULL
                    || item->x() <= leftSelectedItem->x()) {
                    leftSelectedItem = qgraphicsitem_cast<TriggerItem *>(item);
                }

                if (rightSelectedItem == NULL
                    || item->x() >= rightSelectedItem->x()) {
                    rightSelectedItem = qgraphicsitem_cast<TriggerItem *>(item);
                }
            }
        }
    }
    this->setFocusItem(leftSelectedItem);
    drawSelection();
}

void TimelineScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem* item = this->itemAt(mouseEvent->scenePos());
    if (item && item->type() == SelectionItem::Type) {
         // No multiple selection
        if (this->selectedItems().size() == 1) {
            QMessageBox::information(NULL, QString("My App"),
                                     QString("Edit dialog"),
                                     QMessageBox::Close);
        }
    } else {
        QMessageBox::information(NULL, QString("My App"),
                                 QString("Add a new trigger"),
                                 QMessageBox::Close);
        this->addTriggerItem(mouseEvent->scenePos().x() * this->ratio(), 50, 2, "L24bis");
    }
}
