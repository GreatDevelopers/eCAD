#include "cadogrebasewidget.h"

#include <QResizeEvent>
#include <QtX11Extras/QX11Info>
#include <string>
#include <cassert>

CadOgreBaseWidget::CadOgreBaseWidget(Ogre::Root *ogreRoot, OgreEventHandler *ogreEventHandler, QWidget *parent) :
    QGLWidget(parent), mOgreEventHandler(ogreEventHandler), mOgreRoot(ogreRoot), mOgreRenderWindow(NULL)
{
    if (!parent) {
        throw std::runtime_error("Parent widget supplied was uninitialised!");
        // interface requirement
    }

    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    // button event handler

    setFocusPolicy(Qt::StrongFocus);

    Ogre::String winHandle;

    winHandle = Ogre::StringConverter::toString((unsigned long) (QX11Info::display()));
    winHandle += " : ";
    winHandle = Ogre::StringConverter::toString((unsigned long) (QX11Info::appScreen()));
    winHandle += " : ";
    winHandle = Ogre::StringConverter::toString((unsigned long) (winId()));

    Ogre::NameValuePairList params;
    params["parentWindowHandle"] = winHandle;
    params["FSAA"] = Ogre::String("8");

    int w = width();
    int h = height();
    mOgreRenderWindow = mOgreRoot->createRenderWindow("OgreWidget_RenderWindow",
                                                      qMax(w, 640),
                                                      qMax(h, 480),
                                                      false, &params);
    mOgreRenderWindow->setActive(true);
    mOgreRenderWindow->setVisible(true);

    WId ogreWinId = 0x0;
    mOgreRenderWindow->getCustomAttribute("WINDOW", &ogreWinId);
    assert(ogreWinId);
    // guaranteed to be valid due to the way it was created
    QWidget::create(ogreWinId);
    setAttribute(Qt::WA_OpaquePaintEvent);
    // qt won't bother updating the area under/behind the render window
}

Ogre::RenderWindow *CadOgreBaseWidget::getEmbeddedOgreWindow() {
    assert(mOgreRenderWindow); // guaranteed to be valid after construction
    return mOgreRenderWindow;
}


void CadOgreBaseWidget::paintEvent(QPaintEvent *pEvent) {
    this->update();
}


//void CadOgreBaseWidget::resizeEvent(QResizeEvent *rEvent) {
//    if (rEvent) {
//        QWidget::resizeEvent(rEvent);
//    }

//    if (mOgreRenderWindow) {
//        // since the underlying widget has already been updated we can source the resize values from there
//        mOgreRenderWindow->reposition(x(), y());
//        mOgreRenderWindow->resize(width(), height());
//        mOgreRenderWindow->windowMovedOrResized();
//    }
//}

void CadOgreBaseWidget::mousePressEvent(QMouseEvent *event) {
    mOgreEventHandler->ogreMousePressEvent(event);
}


void CadOgreBaseWidget::mouseMoveEvent(QMouseEvent *event) {
    mOgreEventHandler->ogreMouseMoveEvent(event);
}

void CadOgreBaseWidget::update() {
    QWidget::update();
    mOgreRoot->renderOneFrame();
}
