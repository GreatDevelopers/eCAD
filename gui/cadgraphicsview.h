/**
 * cadgraphicsview.h
 *
 * Copyright (C) 2014 GreatDevelopers
 *
 * eCAD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eCAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CADGRAPHICSVIEW_H
#define CADGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QUndoStack>
#include <QUndoView>
#include <QFileDialog>

#include "cadgraphicsscene.h"
#include "cadscriptwidget.h"
#include "cadcommandwidget.h"

class CadGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CadGraphicsView();
    void newFile();
    void cursorMode();
    void showUndoStack();
    void modifySceneRect();
    void setCurrentFile(const QString &fileName);

    CadGraphicsScene *scene;
    QUndoStack *undoStack;
    QUndoView *undoView;
    QAction *undoAction;
    QAction *redoAction;
    CadScriptWidget *scriptWidget;
    CadCommandWidget *commandWidget;

    double scaleFactor;
    bool isPanning;
    bool firstSave;
    QString currentFile;

public slots:
    void drawPoint();
    void drawLine();
    void drawCircle();
    void drawEllipse();
    void drawText();
    void drawArc();
    void drawImage();
    void drawDimHorizontal();
    void drawDimVertical();
    void drawDimRadial();
    void drawDimDiametric();
    void setNoMode();
    void selectWindow();
    void deleteSingleItem();

protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // CADGRAPHICSVIEW_H
