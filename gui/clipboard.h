#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QStack>

#include "gEntity.h"

class MyClipBoard
{
public:
    static MyClipBoard* instance()
    {
        if(!inst)
            inst = new MyClipBoard;
        return inst;
    }

    void push(gEntity* g)
    {
        clips.push(g);
    }

    gEntity* last()
    {
        if(clips.count() == 0) return 0;
        return clips.last();
    }

    gEntity* pop()
    {
        if(clips.count() == 0) return 0;
        return clips.pop();
    }

    bool isempty() const { return clips.empty(); }

private:
    QStack<gEntity*> clips;
    static MyClipBoard* inst;
};

#endif // CLIPBOARD_H
