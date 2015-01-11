/**
 * mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QMdiArea>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>

#include "ui_mainwindow.h"
#include "ui_aboutdialog.h"
#include "cadgraphicsview.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QPainter *painter;
    QPrinter *printer;
    QPixmap image;
    QImage *imageObject;
    CadGraphicsView *view;
    QDialog *aboutDialog;
    typedef QPair<QMdiSubWindow *, CadGraphicsView *> windowViewPair;
    QList<windowViewPair> windowViewList;
    QList<CadScriptWidget *> scriptWidgetList;
    QList<CadCommandWidget *> commandWidgetList;
    QList<QAction *> undoList, redoList;
    QActionGroup *snapActionGroup;

    bool eventFilter(QObject *obj, QEvent *event);
    bool isEntitySelected;
    bool autoSaveCount;
    QString curFileName, fileName;
    int fileNumber;
    QLabel *messageLeft, *messageMiddle;

private slots:
    void newFile();
    void closeActiveWindow();
    void setNoMode();
    void showUndoStack();
    void toggleWidgets();
    void toggleToolBar(bool);
    void hideStatusBar(bool ok);
    bool saveFile(const QString &fileName);
    bool saveFileAs();
    bool save();
    void autoSave();
    void openFile();
    void exportFile(QAction *action);
    void showGrid(bool);
    void zoomIn();
    void zoomOut();
    void panning();
    void selectAll();
    void deselectAll();
    void cutOperation();
    void copyOperation();
    void pasteOperation();
    void toggleMenuActions();
    void selectWindow();
    void deleteItems();
    void toggleActions(bool b);
    void filePrintPreview();
    void closeEvent(QCloseEvent *event);
    void filePrint();
    void print(QPrinter *);
    void showAboutDialog();
    void updateView();
    void setSnapping();
    void setStatusBarMessage();
    CadGraphicsView *createMdiView();
};

#endif // MAINWINDOW_H
