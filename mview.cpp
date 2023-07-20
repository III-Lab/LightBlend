#include "mview.h"
#include <QScrollBar>

MView::MView(QWidget *parent)
{
    init();
}

MView::MView(QGraphicsScene *scene, QWidget *parent)
{
    this->setScene(scene);
    init();
}

void MView::init()
{
    this->setBackgroundBrush(QBrush(Qt::black));
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing
                         | QPainter::HighQualityAntialiasing | QPainter::LosslessImageRendering);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse); // scaled变换时以鼠标为中心变换
}

void MView::wheelEvent(QWheelEvent *event)
{
    // 获取当前的缩放因子
    qreal scaleFactor = transform().m11();

    // 根据鼠标滚轮的滚动方向，增加或减小缩放因子
    if (event->delta() > 0) {
        // 向上滚动放大视图
        scaleFactor *= 1.1;
    } else {
        // 向下滚动缩小视图
        scaleFactor /= 1.1;
    }

    // 限制缩放因子的范围
    if (scaleFactor < 0.2) {
        scaleFactor = 0.2;
    } else if (scaleFactor > 5) {
        scaleFactor = 5;
    }

    // 进行缩放操作
    QTransform transform;
    transform.scale(scaleFactor, scaleFactor);
    setTransform(transform);

    QGraphicsView::wheelEvent(event);
}


void MView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        // 记录右键按下的起始位置
        m_lastPos = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}

void MView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton) {
        // 计算当前位置和上一次位置的差值
        QPoint delta = event->pos() - m_lastPos;

        // 滚动视图
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        // 更新上一次位置
        m_lastPos = event->pos();
    }

    QGraphicsView::mouseMoveEvent(event);
}

void MView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        // 清除起始位置信息
        m_lastPos = QPoint();

//        QPointF p1 = mapToScene(event->pos());
//        QPoint p3(int(p1.x()), int(p1.y()));
//        QPointF p2 = pixmapItem->mapToScene(p3);
//        double w = p2.x();
//        double h = p2.y();
    }

    QGraphicsView::mouseReleaseEvent(event);
}
