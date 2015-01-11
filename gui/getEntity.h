/**
 * getEntity.h
 *
 * Copyright (C) 2014 GreatDevelopers
 *
 * eCAD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eCAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
