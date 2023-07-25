#ifndef MVIEW_H
#define MVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class MView : public QGraphicsView
{
public:
    explicit MView(QWidget *parent = nullptr);
    explicit MView(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:

    void init(void);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QPoint m_lastPos;
};

#endif // MVIEW_H
