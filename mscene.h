#ifndef MSCENE_H
#define MSCENE_H

#include <QGraphicsScene>

class MScene : public QGraphicsScene
{
    Q_OBJECT
public:
     MScene(QObject *parent = nullptr);
    ~MScene();
    MScene& operator=(const MScene&) = delete;
private:
    QByteArray getItemClassName(QByteArray &name) const;

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);


//    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
public slots:
    void slotGetMaskCenter(QPointF, int);
signals:
    void sigGetMaskCenter(QPointF, int);
};

#endif // MSCENE_H
