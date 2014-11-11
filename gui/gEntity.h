#ifndef GENTITY_H
#define GENTITY_H

#include <QGraphicsItem>

class gEntity : public QObject , public QGraphicsItem
{
public:
    gEntity(QObject *parent = 0) : QObject(parent) {}
    virtual ~gEntity() {}

    virtual gEntity *clone() { return 0; }
};

#endif // GENTITY_H
