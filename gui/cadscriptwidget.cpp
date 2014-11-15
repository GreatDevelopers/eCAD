#include "cadscriptwidget.h"

CadScriptWidget::CadScriptWidget() {
    setWindowTitle("Scripting Console");

    // widgets and buttons
    tEdit = new QTextEdit;
    w = new QWidget;
    newSript = new QPushButton("New");
    loadScript = new QPushButton("Load");
    saveScript = new QPushButton("Save");
    clearScript = new QPushButton("Clear");
    executeScript = new QPushButton("Execute");

    // layouts
    hBox = new QHBoxLayout;
    vBox = new QVBoxLayout;

    hBox->addWidget(newSript);
    hBox->addWidget(loadScript);
    hBox->addWidget(saveScript);
    hBox->addWidget(clearScript);
    hBox->addWidget(executeScript);

    vBox->addLayout(hBox);
    vBox->addWidget(tEdit);

    w->setLayout(vBox);
    setWidget(w);

    jsEngine = new QJSEngine;

    connect(executeScript, SIGNAL(pressed()), this, SLOT(execute()));
    setupJSEngine();
}

void CadScriptWidget::setupJSEngine() {
    QJSValue cadContext = jsEngine->newQObject(this);
    jsEngine->globalObject().setProperty("cad", cadContext);
}

void CadScriptWidget::execute() {
    QString text = tEdit->toPlainText();
    QJSValue result = jsEngine->evaluate(text);
}

void CadScriptWidget::drawPoint(int x, int y) {
    qDebug() << x << "," << y;
}
