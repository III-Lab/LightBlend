#include "displaywidget.h"
#include <QPainter>
DisplayWidget::DisplayWidget(QWidget *parent) : QWidget(parent)
{
    image = QImage(256,256,QImage::Format_RGB32);
    image.fill(Qt::black);
}

void DisplayWidget::paintEvent(QPaintEvent *)
{
    if (image.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.drawImage(this->rect(), image);
}

void DisplayWidget::slotGetImage(const QImage &_image)
{
    image = _image;
    update();
}
