#ifndef GETENTITY_H
#define GETENTITY_H

#include <QGraphicsItem>

class getEntity : public QObject, public QGraphicsItem
{
public:
   getEntity(QObject *parent = 0) : QObject(parent) {}
   virtual ~getEntity() {}

   virtual getEntity *clone(int i)
   {
       return 0;
   }

   int id;
};

#endif // GENTITY_H
