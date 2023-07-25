#include "mitem.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include "mainwindow.h"

#define Item_Width       40
#define Item_Height      40
#define Item_Width_Div2  20
#define Item_Height_Div2 20

#include <QPainter>
MItem::MItem(QGraphicsObject *parent)
    : QGraphicsObject(parent)
{
    this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    //this->setAcceptHoverEvents(true);
    this->setFlags(  QGraphicsItem::ItemIsMovable
                   | QGraphicsItem::ItemIsSelectable
                   | QGraphicsItem::ItemIsFocusable);

}

MItem::~MItem()
{

}

QString MItem::nameString() const
{
    return name;
}

void MItem::drawSelectedRect(QPainter *painter)
{
    Q_UNUSED(painter)
//    QRectF rect = this->boundingRect();

//    painter->setPen(QPen(QColor(8,255,200), 0, Qt::DashLine));
//    painter->setBrush(Qt::NoBrush);

//    painter->drawRect(rect.adjusted(-1, -1, 1, 1));
}

void MItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
}

void MItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
}

void MItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = mapToScene(event->pos());
    emit sigGetMaskCenter(pos, radius);
//    QGraphicsScene* scene = this->scene();
    QGraphicsObject::mouseReleaseEvent(event);
}

void MItem::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        deleteItem();
        return;
    }

    QGraphicsItem::keyPressEvent(event);
}
#include <QMenu>
void MItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->isSelected()) return;
    if (event->button() == Qt::RightButton) {
        if (isSelected()) {
            QMenu menu;

            // 添加菜单项和相应的操作
            QAction *deleteAction = menu.addAction("删除");
            connect(deleteAction, &QAction::triggered, this, &MItem::deleteItem);

            // 在鼠标位置显示菜单
            menu.exec(event->screenPos());
        }
    }
    else {
        QGraphicsItem::mousePressEvent(event);
    }
}

void MItem::deleteItem(void)
{
    // 执行删除操作
    this->scene()->removeItem(this);
    delete this;
}

/**********
 *                   Rect
**********/

MItemRect::MItemRect(QGraphicsObject *parent)
    : MItem(parent)
{
    name = "Rect";
}

QRectF MItemRect::boundingRect() const
{
    return QRectF(-Item_Width_Div2,-Item_Height_Div2,Item_Width,Item_Height);
}

void MItemRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::black);
    painter->drawRect(this->boundingRect());

    if(this->isSelected())
        this->drawSelectedRect(painter);
}

/**********
 *                   Ellipse
**********/

MItemEllipse::MItemEllipse(QGraphicsObject *parent)
    : MItem(parent)
{
    name = "Ellipse";
    radius = Item_Width_Div2;
}

QRectF MItemEllipse::boundingRect() const
{
    return QRectF(-radius,-radius,radius*2,radius*2);
}

void MItemEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::black);
    painter->drawEllipse(this->boundingRect());

    if(this->isSelected())
        this->drawSelectedRect(painter);
}

#include <QInputDialog>
void MItemEllipse::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    bool ok;
    QString text = QInputDialog::getText(nullptr, nullptr,
                                        tr("Input Radius:"), QLineEdit::Normal,nullptr, &ok);
    if (ok && !text.isEmpty())
    {
        radius = text.toInt();
        update();
    }
}


/**********
 *                   Text
**********/
MItemText::MItemText(QGraphicsObject *parent)
    : MItem(parent)
{
    name = "Text";
    m_text = name;
    m_font.setPixelSize(Item_Width_Div2);
}

QRectF MItemText::boundingRect() const
{
    int size = m_font.pixelSize();
    int count = m_text.size();
    return QRectF(-size/2*count/2,-size/2,size*count/2,size);
}

void MItemText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget); Q_UNUSED(option);

    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setFont(m_font);
    painter->drawText(this->boundingRect(), m_text);

    if(this->isSelected())
        this->drawSelectedRect(painter);
}

#include <QInputDialog>
void MItemText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    bool ok;
    QString text = QInputDialog::getText(nullptr, nullptr,
                                        tr("Input:"), QLineEdit::Normal,nullptr, &ok);
    if (ok && !text.isEmpty())
    {
        m_text = text;
        update();
    }
}


/**********
 *                   Mask10
**********/
MItemMask10::MItemMask10(QGraphicsObject *parent)
    : MItem(parent)
{
    radius = 10;
}

QRectF MItemMask10::boundingRect() const
{
    return QRectF(-radius,-radius,radius*2,radius*2);
}

void MItemMask10::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::white);
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(this->boundingRect());

    if(this->isSelected())
        this->drawSelectedRect(painter);
}

void MItemMask10::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    bool ok;
    QString text = QInputDialog::getText(nullptr, nullptr,
                                        tr("Input Radius:"), QLineEdit::Normal,nullptr, &ok);
    if (ok && !text.isEmpty())
    {
        radius = text.toInt();
        update();
    }
}



/**********
 *                   Mask20
**********/
MItemMask20::MItemMask20(QGraphicsObject *parent)
    : MItem(parent)
{
    radius = 20;
}

QRectF MItemMask20::boundingRect() const
{
    return QRectF(-radius,-radius,radius*2,radius*2);
}

void MItemMask20::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::white);
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(this->boundingRect());

    if(this->isSelected())
        this->drawSelectedRect(painter);
}

void MItemMask20::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    bool ok;
    QString text = QInputDialog::getText(nullptr, nullptr,
                                        tr("Input Radius:"), QLineEdit::Normal,nullptr, &ok);
    if (ok && !text.isEmpty())
    {
        radius = text.toInt();
        update();
    }
}


/**********
 *                   Mask30
**********/
MItemMask30::MItemMask30(QGraphicsObject *parent)
    : MItem(parent)
{
    radius = 30;
}

QRectF MItemMask30::boundingRect() const
{
    return QRectF(-radius,-radius,radius*2,radius*2);
}

void MItemMask30::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::white);
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(this->boundingRect());

    if(this->isSelected())
        this->drawSelectedRect(painter);
}

void MItemMask30::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    bool ok;
    QString text = QInputDialog::getText(nullptr, nullptr,
                                        tr("Input Radius:"), QLineEdit::Normal,nullptr, &ok);
    if (ok && !text.isEmpty())
    {
        radius = text.toInt();
        update();
    }
}


/**********
 *                   Mask40
**********/
MItemMask40::MItemMask40(QGraphicsObject *parent)
    : MItem(parent)
{
    radius = 40;
}

QRectF MItemMask40::boundingRect() const
{
    return QRectF(-radius,-radius,radius*2,radius*2);
}

void MItemMask40::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::white);
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(this->boundingRect());

    if(this->isSelected())
        this->drawSelectedRect(painter);
}

void MItemMask40::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    bool ok;
    QString text = QInputDialog::getText(nullptr, nullptr,
                                        tr("Input Radius:"), QLineEdit::Normal,nullptr, &ok);
    if (ok && !text.isEmpty())
    {
        radius = text.toInt();
        update();
    }
}


/**********
 *                   Mask50
**********/
MItemMask50::MItemMask50(QGraphicsObject *parent)
    : MItem(parent)
{
    radius = 50;
}

QRectF MItemMask50::boundingRect() const
{
    return QRectF(-radius,-radius,radius*2,radius*2);
}

void MItemMask50::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::white);
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(this->boundingRect());

    if(this->isSelected())
        this->drawSelectedRect(painter);
}

void MItemMask50::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    bool ok;
    QString text = QInputDialog::getText(nullptr, nullptr,
                                        tr("Input Radius:"), QLineEdit::Normal,nullptr, &ok);
    if (ok && !text.isEmpty())
    {
        radius = text.toInt();
        update();
    }
}
