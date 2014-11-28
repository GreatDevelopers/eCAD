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
    void ellipse(qreal, qreal, qreal, qreal);
    void text(qreal, qreal, QString);
    void arc(qreal, qreal, qreal, qreal, qreal, qreal);
};

#endif // CADSCRIPTWIDGET_H
