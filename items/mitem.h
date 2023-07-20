#ifndef MITEM_H
#define MITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
class MItem : public QGraphicsObject
{
    Q_OBJECT
public:
    MItem(QGraphicsObject *parent = nullptr);
    ~MItem();

    QString nameString() const;

    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) = 0;

protected:
    virtual void drawSelectedRect(QPainter *painter); /* 画虚线框 */
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private slots:
    void deleteItem(void);

protected:
    QString name = "base";
};


class MItemRect : public MItem
{
    Q_OBJECT
public:
    MItemRect(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};


class MItemEllipse : public MItem
{
    Q_OBJECT
public:
    MItemEllipse(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    int radius;
};

#include <QFont>
class MItemText : public MItem
{
    Q_OBJECT
public:
    MItemText(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QFont m_font;
    QString m_text;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};


class MItemMask10 : public MItem
{
    Q_OBJECT
public:
    MItemMask10(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    int radius;
};

class MItemMask20 : public MItem
{
    Q_OBJECT
public:
    MItemMask20(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    int radius;
};

class MItemMask30 : public MItem
{
    Q_OBJECT
public:
    MItemMask30(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    int radius;
};

class MItemMask40 : public MItem
{
    Q_OBJECT
public:
    MItemMask40(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    int radius;
};

class MItemMask50 : public MItem
{
    Q_OBJECT
public:
    MItemMask50(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    int radius;
};

#endif // MITEM_H
