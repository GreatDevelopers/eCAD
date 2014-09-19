#ifndef CADOGREBASEWIDGET_H
#define CADOGREBASEWIDGET_H

#include <GL/glew.h>
#include <QGLWidget>

#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>

#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISMouse.h>
#include <OISKeyboard.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

using namespace Ogre;
using namespace OgreBites;

class CadOgreBaseWidget : public QGLWidget, public FrameListener,
    public WindowEventListener,
//    public OIS::KeyListener,
//    public OIS::MouseListener,
    SdkTrayListener
{
  public:
    CadOgreBaseWidget();
    virtual ~CadOgreBaseWidget();

    virtual void go();

  protected:
    Root *mRoot;
    Camera *mCamera;
    RenderWindow *mRenderWindow;
    RenderSystem *mRenderSystem;
    SceneManager *mSceneManager;
    String mResourcesCfg;
    String mPluginsCfg;

    SdkCameraMan *mCameraMan;

  protected:
    virtual bool setup();
    virtual bool configure();

    virtual void createScene() = 0;
    virtual void destroyScene();
    virtual void createCamera();

    virtual void chooseSceneManager();
    virtual void createViewports();
    virtual void setupResources();
    virtual void loadResources();

};

#endif //CADOGREBASEWIDGET_H
