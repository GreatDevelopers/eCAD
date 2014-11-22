#pragma once

#include "dxfrw_intf.h"
#include <QPaintEvent>
#include <QPainter>
#include <cadgraphicsscene.h>
#include <cadgraphicsscene.h>
#include <QtCore>
#include "mainwindow.h"

class DXFimpl : public MainWindow, public DRW_InterfaceImpl {

    virtual void addLine(const DRW_Line &data);
    virtual void addCircle(const DRW_Circle& data);
public:
    DXFimpl(CadGraphicsScene *scene);

    CadGraphicsScene* getScene();
private:
    CadGraphicsScene *newScene;
    CadGraphicsView *view;
};
