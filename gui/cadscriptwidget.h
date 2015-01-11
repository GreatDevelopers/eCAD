/**
 * cadscriptwidget.h
 *
 * Copyright (C) 2014 GreatDevelopers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CADSCRIPTWIDGET_H
#define CADSCRIPTWIDGET_H

#include <QDockWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJSEngine>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

#include "cadgraphicsscene.h"

class CadScriptWidget : public QDockWidget
{
    Q_OBJECT

public:
    CadScriptWidget();
    void *getCurrentScene(CadGraphicsScene *);

private:
    void setupJSEngine();
    void setCurrentFile(const QString &fileName);
    void toggleButtons(bool b);

    QJSEngine *jsEngine;
    QTextEdit *tEdit;
    QWidget *w;
    QPushButton *newScript;
    QPushButton *loadScript;
    QPushButton *saveScript;
    QPushButton *clearScript;
    QPushButton *executeScript;
    QHBoxLayout *hBox;
    QVBoxLayout *vBox;
    CadGraphicsScene *currentScene;
    QSpacerItem *verticalSpacer;
    Point *pointItem;
    Line *lineItem;
    Circle *circleItem;
    Ellipse *ellipseItem;
    Text *textItem;
    Arc *arcItem;

    int id;
    bool isNew;
    bool firstRun;
    QString currentFile;

private slots:
    void newScriptFxn();
    void loadScriptFxn();
    void autoSave();
    bool saveScriptFxn(const QString &fileName);
    bool saveAsScriptFxn();
    void clearScriptFxn();
    void executeScriptFxn();

public slots:
    void point(qreal, qreal);
    void line(qreal, qreal, qreal, qreal);
    void circle(qreal, qreal, qreal);
    void ellipse(qreal, qreal, qreal, qreal, qreal);
    void text(qreal, qreal, QString);
    void arc(qreal, qreal, qreal, qreal, qreal, qreal);
};

#endif // CADSCRIPTWIDGET_H
