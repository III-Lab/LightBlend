#ifndef OBSERVERMITEM_H
#define OBSERVERMITEM_H

#include <QObject>

class ObserverMItem : public QObject
{
    Q_OBJECT
public:
    explicit ObserverMItem(QObject *parent = nullptr);

signals:

public slots:
};

#endif // OBSERVERMITEM_H
