#ifndef MDICHILD_H
#define MDICHILD_H

#include <QMdiSubWindow>
#include "cadgraphicsview.h"
#include "cadgraphicsscene.h"

class MdiChild : public QMdiSubWindow
{
public:
    MdiChild(QWidget *parent = NULL);
    ~MdiChild();

    void newFile();

protected:
    virtual QSize sizeHint() const;

private:
    QString fileName;
    bool isUntitled;
    CadGraphicsView *view;
    CadGraphicsScene *scene;
};

#endif // MDICHILD_H
