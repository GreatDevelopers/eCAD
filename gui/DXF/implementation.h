#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H

#include "dxfrw_intf.h"
#include <QPaintEvent>
#include <QPainter>
#include <cadgraphicsscene.h>
#include <cadgraphicsscene.h>
#include <QtCore>
#include "mainwindow.h"

//class MainWindow;
class implementation : public MainWindow, public DRW_InterfaceImpl {

    virtual void addLine(const DRW_Line &data);
    virtual void addCircle(const DRW_Circle& data);
public:
    implementation(CadGraphicsScene *scene);

    CadGraphicsScene* scene();
private:
    CadGraphicsScene *newScene;
    CadGraphicsView *view;
};
#endif // IMPLEMENTATION_H
