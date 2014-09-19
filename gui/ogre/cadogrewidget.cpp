#include "cadogrewidget.h"

CadOgreWidget::CadOgreWidget()
{
  go();
}

CadOgreWidget::~CadOgreWidget()
{

}

void CadOgreWidget::createScene()
{
  mSceneManager->setAmbientLight(ColourValue(0.25,0.25,0.25));

  Light *pointLight = mSceneManager->createLight("pointLight");
  pointLight->setType(Light::LT_POINT);
  pointLight->setPosition(Vector3(250,150,250));
  pointLight->setDiffuseColour(ColourValue::White);
  pointLight->setSpecularColour(ColourValue::White);
}

void CadOgreWidget::destroyScene()
{

}
