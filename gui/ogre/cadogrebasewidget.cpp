#include "cadogrebasewidget.h"

CadOgreBaseWidget::CadOgreBaseWidget():
  mRoot(0),
  mCamera(0),
  mSceneManager(0),
  mRenderWindow(0),
  mResourcesCfg(StringUtil::BLANK),
  mPluginsCfg(StringUtil::BLANK),
  mCameraMan(0)
{

}

CadOgreBaseWidget::~CadOgreBaseWidget()
{
  if(mCameraMan)
    delete mCameraMan;

  delete mRoot;
}

bool CadOgreBaseWidget::configure()
{
  RenderSystemList::const_iterator availableRendererIt = mRoot->getAvailableRenderers().begin();

  while (availableRendererIt != mRoot->getAvailableRenderers().end()) {
      String rName = (*availableRendererIt)->getName();
      if (rName == "OpenGL Rendering Subsystem") {
          break;
      }
      ++availableRendererIt;
  }

  if (availableRendererIt == mRoot->getAvailableRenderers().end()) {
      throw std::runtime_error("We were unable to find the OpenGL renderer in ogre's list, cannot continue");
  }

  // use the OpenGL renderer in the root config
  mRenderSystem = *availableRendererIt;
  mRoot->setRenderSystem(mRenderSystem);
  //mRenderWindow = mRoot->initialise(false);
  mRenderWindow = mRoot->initialise(true,"RenderWindow");
  return true;
}

void CadOgreBaseWidget::chooseSceneManager()
{
  mSceneManager = mRoot->createSceneManager(ST_GENERIC);
}

void CadOgreBaseWidget::createCamera()
{
  mCamera = mSceneManager->createCamera("PlayerCam");
  mCamera->setPosition(Vector3(0,0,100));
  mCamera->lookAt(Vector3(0,0,-300));
  mCamera->setNearClipDistance(5);

  mCameraMan = new SdkCameraMan(mCamera);
}

void CadOgreBaseWidget::destroyScene()
{

}

void CadOgreBaseWidget::createViewports()
{
  Viewport *vp = mRenderWindow->addViewport(mCamera);
  vp->setBackgroundColour(ColourValue::Blue);
  mCamera->setAspectRatio(Real(vp->getActualWidth())/Real(vp->getActualHeight()));
}

void CadOgreBaseWidget::setupResources()
{
  ConfigFile cf;
  cf.load(mResourcesCfg);

  ConfigFile::SectionIterator seci = cf.getSectionIterator();

  String secName, typeName, archName;
  while (seci.hasMoreElements())
  {
    secName = seci.peekNextKey();
    ConfigFile::SettingsMultiMap *settings = seci.getNext();
    ConfigFile::SettingsMultiMap::iterator i;
    for(i = settings->begin(); i != settings->end(); ++i)
    {
      typeName = i->first;
      archName = i->second;
      ResourceGroupManager::getSingleton().addResourceLocation(
            archName, typeName, secName);
    }
  }
}

void CadOgreBaseWidget::loadResources()
{
  ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void CadOgreBaseWidget::go()
{
  mResourcesCfg = "resources.cfg";
  mPluginsCfg = "plugins.cfg";

  if(!setup())
    return;

  mRoot->startRendering();

  destroyScene();
}

bool CadOgreBaseWidget::setup()
{
  mRoot = new Root(mPluginsCfg);
  setupResources();
  bool carryOn = configure();
  if(!carryOn)
    return false;

  chooseSceneManager();
  createCamera();
  createViewports();

  TextureManager::getSingleton().setDefaultNumMipmaps(5);
  loadResources();

  createScene();
  return true;
}
