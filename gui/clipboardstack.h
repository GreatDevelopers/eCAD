/**
 * clipboardstack.h
 *
 * Copyright (C) 2014  GreatDevelopers
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

#ifndef CLIPBOARDSTACK_H
#define CLIPBOARDSTACK_H

#include <QStack>

#include "getEntity.h"

class clipboardStack
{
public:
    static clipboardStack *instance()
    {
        if (!inst)
            inst = new clipboardStack;

        return inst;
    }

    void push(getEntity *entity)
    {
        clips.push(entity);
    }

    getEntity *pasteEntity()
    {
        if (clips.count() == 0)
            return 0;

        return clips.last();
    }

    getEntity *pop()
    {
        if (clips.count() == 0)
            return 0;

        return clips.pop();
    }

    bool isEmpty() const
    {
        return clips.empty();
    }

private:
    QStack<getEntity *> clips;
    static clipboardStack *inst;
};

#endif // CLIPBOARDSTACK_H
