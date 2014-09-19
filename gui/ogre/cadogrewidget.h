#ifndef CADOGREWIDGET_H
#define CADOGREWIDGET_H

#include "cadogrebasewidget.h"

class CadOgreWidget: public CadOgreBaseWidget
{
  public:
    CadOgreWidget();
    virtual ~CadOgreWidget();

  protected:
    virtual void createScene();
    virtual void destroyScene();
};


#endif //CADOGREWIDGET_H
