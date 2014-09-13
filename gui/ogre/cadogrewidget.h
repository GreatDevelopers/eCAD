#ifndef CADOGREWIDGET_H
#define CADOGREWIDGET_H

#include <Ogre.h>

#include <QWidget>
#include "cadogrebasewidget.h"
#include "cadcameraman.h"

#include "eventhandler.h"


class CadOgreWidget : public QWidget, public OgreEventHandler
{
    Q_OBJECT

public: // interface
    CadOgreWidget();
    ~CadOgreWidget();

    void setupRenderSystem();
    //void setupResources();
    void createQtWidgets();
    void createScene();

private:
    CadOgreBaseWidget *mOgreWidget;

    Ogre::Root *mOgreRoot;
    Ogre::RenderWindow *mRenderWindow;
    Ogre::Viewport *mOgreViewport;
    Ogre::RenderSystem *mRenderSystem;
    Ogre::SceneManager *mSceneManager;
    Ogre::Camera *mCamera;

    OgreBites::QCameraMan* mCameraMan;

    int mMouseMoveXOld;
    int mMouseMoveYOld;

    // button event handler
private slots:
    void onZoomIn();
    void onZoomOut();

    // implements IQOgreEventHandler
    virtual void ogrePaintEvent(QPaintEvent *pEvent) {}
    virtual void ogreResizeEvent(QResizeEvent *rEvent) {}
    virtual void ogreUpdate() {}
    virtual void ogreMousePressEvent(QMouseEvent *event);
    virtual void ogreMouseMoveEvent(QMouseEvent *event);
};

#endif // CADOGREWIDGET_H
