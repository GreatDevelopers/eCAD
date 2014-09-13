#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>

class OgreEventHandler
{
public:
    virtual void ogrePaintEvent(QPaintEvent* event) = 0;
    virtual void ogreResizeEvent(QResizeEvent* event) = 0;
    virtual void ogreUpdate() = 0;
    virtual void ogreMousePressEvent(QMouseEvent* event) = 0;
    virtual void ogreMouseMoveEvent(QMouseEvent* event) = 0;
};

#endif // EVENTHANDLER_H
