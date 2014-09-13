#ifndef CADOGREBASEWIDGET_H
#define CADOGREBASEWIDGET_H

#include <Ogre.h>
#include <GL/glew.h>
#include <QGLWidget>

#include "eventhandler.h"

class CadOgreBaseWidget : public QGLWidget
{
    Q_OBJECT

public:
    CadOgreBaseWidget(Ogre::Root *ogreRoot, OgreEventHandler *ogreEventHandler, QWidget *parent);
    Ogre::RenderWindow *getEmbeddedOgreWindow();

protected:
    virtual void paintEvent(QPaintEvent *pEvent);
//    virtual void resizeEvent(QResizeEvent *rEvent);
    virtual void update();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    Ogre::Root *mOgreRoot;
    Ogre::RenderWindow *mOgreRenderWindow;
    OgreEventHandler *mOgreEventHandler;

};

#endif // CADOGREBASEWIDGET_H
