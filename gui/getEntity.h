#ifndef GETENTITY_H
#define GETENTITY_H

#include <QGraphicsItem>

class getEntity : public QObject, public QGraphicsItem
{
public:
   getEntity(QObject *parent = 0) : QObject(parent) {}
   virtual ~getEntity() {}

   virtual getEntity *clone()
   {
       return 0;
   }
};

#endif // GENTITY_H
