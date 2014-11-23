#pragma once

#include <QPaintEvent>
#include <QPainter>
#include <cadgraphicsscene.h>
#include <cadgraphicsscene.h>
#include <QtCore>
#include "mainwindow.h"
extern "C" {
#include <dwg_api.h>
#include <dwg.h>
}
class DWGimpl : public MainWindow
{
    Dwg_Data data;
public:
    void addLine(dwg_object* obj);
    void addCircle(dwg_object* obj);
    void addArc(dwg_object* obj);
    void addText(dwg_object* obj);
    void output_BLOCK_HEADER(dwg_object_ref* ref);
    void ReadDWG(dwg_data* dwg) ;
    void OutputObject(dwg_object* obj);
    int readFile(char* filename);
public:
    DWGimpl(CadGraphicsScene *scene);

    CadGraphicsScene* getScene();
private:
    CadGraphicsScene *newScene;
    CadGraphicsView *view;
};
