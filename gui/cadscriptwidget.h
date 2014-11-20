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

#include "cadgraphicsview.h"

class CadScriptWidget : public QDockWidget
{
    Q_OBJECT

public:
    CadScriptWidget();

private:
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
    CadGraphicsView view;
    QSpacerItem *verticalSpacer;
    void setupJSEngine();
    void setCurrentFile(const QString &fileName);
    void toggleButtons(bool b);

    bool isNew;
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
    void drawPoint(int, int);
};

#endif // CADSCRIPTWIDGET_H
