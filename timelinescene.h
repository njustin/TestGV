#ifndef TIMELINESCENE_H
#define TIMELINESCENE_H

#include <QGraphicsScene>

#include "triggeritem.h"
#include "selectionitem.h"

#define MIN_SEQUENCE 0
#define MAX_SEQUENCE 32

class TimelineScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit TimelineScene(QObject *parent = 0);

    qreal ratio();
    qreal maxLength();

    bool addTriggerItem(qreal x, int address, int sequence, QString label);
    void removeTriggerItem(TriggerItem *item);

    TriggerItem* getTriggerItemBefore(TriggerItem *item);
    TriggerItem* getTriggerItemAfter(TriggerItem *item);
    SelectionItem* selectionItem;

private:
    qreal mRatio;
    qreal mMaxLength;
    QList<TriggerItem*> triggerItems;
    QList<TriggerItem*> sequences[MAX_SEQUENCE];

    TriggerItem* leftSelectedItem;
    TriggerItem* rightSelectedItem;

    void drawSelection();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:

public slots:
    void updateSelection();
};

#endif // TIMELINESCENE_H
