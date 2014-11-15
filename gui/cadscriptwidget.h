#ifndef CADSCRIPTWIDGET_H
#define CADSCRIPTWIDGET_H

#include <QDockWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJSEngine>
#include <QDebug>

#include "cadgraphicsview.h"

class CadScriptWidget : public QDockWidget {
        Q_OBJECT

    public:
        CadScriptWidget();

    private:
        QJSEngine* jsEngine;
        QTextEdit* tEdit;
        QWidget* w;
        QPushButton* newSript;
        QPushButton* loadScript;
        QPushButton* saveScript;
        QPushButton* clearScript;
        QPushButton* executeScript;
        QHBoxLayout* hBox;
        QVBoxLayout* vBox;
        CadGraphicsView view;
        void setupJSEngine();

    private slots:
        void execute();

    public slots:
        void drawPoint(int, int);
};

#endif // CADSCRIPTWIDGET_H
