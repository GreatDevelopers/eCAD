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
