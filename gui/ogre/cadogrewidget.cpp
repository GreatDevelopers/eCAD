#include "cadogrewidget.h"
#include "OGRE/OgreDepthBuffer.h"
#include <OGRE/OgreHardwareUniformBuffer.h>
#include <OGRE/OgreHardwareCounterBuffer.h>
#include "OGRE/RenderSystems/GL/OgreGLPlugin.h"
#include "cadogrebasewidget.h"

//void CadOgreWidget::setupResources(void)
//{
//    // Load resource paths from config file
//    Ogre::ConfigFile cf;
//    cf.load("resources.cfg");

//    // Go through all sections & settings in the file
//    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

//    Ogre::String secName, typeName, archName;
//    while (seci.hasMoreElements()) {
//        secName = seci.peekNextKey();
//        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
//        Ogre::ConfigFile::SettingsMultiMap::iterator i;
//        for (i = settings->begin(); i != settings->end(); ++i) {
//            typeName = i->first;
//            archName = i->second;
//            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
//        }
//    }
//}

void CadOgreWidget::setupRenderSystem()
{
    // look for the openGL renderer in Ogre
    Ogre::RenderSystemList::const_iterator availableRendererIt = mOgreRoot->getAvailableRenderers().begin();

    while (availableRendererIt != mOgreRoot->getAvailableRenderers().end()) {
        Ogre::String rName = (*availableRendererIt)->getName();
        if (rName == "OpenGL Rendering Subsystem") {
            break;
        }
        ++availableRendererIt;
    }

    if (availableRendererIt == mOgreRoot->getAvailableRenderers().end()) {
        throw std::runtime_error("We were unable to find the OpenGL renderer in ogre's list, cannot continue");
    }

    // use the OpenGL renderer in the root config
    mRenderSystem = *availableRendererIt;
    mOgreRoot->setRenderSystem(mRenderSystem);
    mRenderWindow = mOgreRoot->initialise(false);
}


void CadOgreWidget::createScene()
{
    mSceneManager = mOgreRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
    mCamera = mSceneManager->createCamera("QOgreWidget_Cam");
    mCamera->setPosition(1.0, 1.0, 200);
    mCamera->setAutoAspectRatio(true);
    mCameraMan = new OgreBites::QCameraMan(mCamera);
    mCameraMan->setCamera(mCamera);

    mOgreViewport = mOgreWidget->getEmbeddedOgreWindow()->addViewport(mCamera);

//    this->resize(640, 480);
//    this->setWindowTitle("QOgreWidget demo");

    // Set the scene's ambient light
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    // Create an Entity
//    Ogre::Entity *ogreHead = mSceneManager->createEntity("Head", "ogrehead.mesh");

//    // Create a SceneNode and attach the Entity to it
//    Ogre::SceneNode *headNode = mSceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
//    headNode->attachObject(ogreHead);

    Ogre::ManualObject *line = mSceneManager->createManualObject("Line");
    line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_POINT_LIST);
    line->position(-100,-100,-100);
    line->position(100,100,100);
    line->end();

    mSceneManager->getRootSceneNode()->attachObject(line);

    // Create a Light and set its position
    Ogre::Light *light = mSceneManager->createLight("MainLight");
    light->setPosition(20.0f, 80.0f, 50.0f);
}

void CadOgreWidget::createQtWidgets()
{
    mOgreWidget = new CadOgreBaseWidget(mOgreRoot, this, this);
    mOgreWidget->resize(this->size());
}

void CadOgreWidget::onZoomIn() {
    mCamera->moveRelative(Ogre::Vector3(0, 0, -10.0));
}


void CadOgreWidget::onZoomOut() {
    mCamera->moveRelative(Ogre::Vector3(0, 0, 10));
}


void CadOgreWidget::ogreMousePressEvent(QMouseEvent *event) {
    mCameraMan->injectMouseDown(*event);
}


void CadOgreWidget::ogreMouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        std::cout << "mouse Moved (left button pressed)";
    } else if (event->buttons() & Qt::RightButton) {
        std::cout << "mouse Moved (right button pressed)";
    }
    int x = event->x();
    int y = event->y();
    int dx = mMouseMoveXOld - x;
    int dy = mMouseMoveYOld - y;

    mMouseMoveXOld = x;
    mMouseMoveYOld = y;

    mCameraMan->injectMouseMove(*event);
}


CadOgreWidget::CadOgreWidget(/*QWidget *parent*/) :
//    QWidget(parent),
    mOgreRoot(new Ogre::Root("plugins.cfg")) {

    //setupResources();
    setupRenderSystem();

    createQtWidgets();
    createScene();

    mOgreRoot->renderOneFrame();
//    this->show(); // give focus to our application and make it visible
}


CadOgreWidget::~CadOgreWidget() {
    delete mOgreRoot;
    delete mOgreWidget;
}

