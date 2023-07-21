#include "mscene.h"

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "mitem.h"
#include "reflect.h"
MScene::MScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->setBackgroundBrush(QColor("#FFFFFF"));
}

MScene::~MScene()
{

}

QByteArray MScene::getItemClassName(QByteArray &name) const
{
    const char *c = "MItem";
    return QByteArray(c) + name;
}

void MScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("items"))
        event->setAccepted(true);
}

void MScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("items"))
        event->setAccepted(true);
}

void MScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QByteArray data = event->mimeData()->data("items");
    QByteArray classname = getItemClassName(data);

//    qDebug() << classname;
    MItem *item = (MItem*)Reflect::createObject(classname);

    if(item)
    {
        connect(item, &MItem::sigGetMaskCenter, this, &MScene::slotGetMaskCenter);
        qDebug() << QString("Add a %1 item.").arg(item->nameString());
        item->setPos(event->scenePos());
        this->addItem(item);
    }
    else
        qDebug() << "No item.";
}

void MScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MScene::slotGetMaskCenter(QPointF pos, int r)
{
    emit sigGetMaskCenter(pos,r);
}

