#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QObject>
#include <QWidget>

class DisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void slotGetImage(const QImage &image);
private:
    QImage image;
};

#endif // DISPLAYWIDGET_H
