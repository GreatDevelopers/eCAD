#include "dxfimpl.h"

DXFimpl::DXFimpl(CadGraphicsScene *scene)
    : newScene(scene)
{
}


void DXFimpl::addLine(const DRW_Line &data)
{
    newScene->addLine(data.basePoint.x, data.basePoint.y,
                      data.secPoint.x, data.secPoint.y);
}

void DXFimpl::addCircle(const DRW_Circle& data)
{

}

CadGraphicsScene* DXFimpl::getScene()
{
    return newScene;
}
