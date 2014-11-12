#include "cadscriptwidget.h"

CadScriptWidget::CadScriptWidget()
{
    setWindowTitle("Scripting Console");

    // widgets and buttons
    QTextEdit *tEdit = new QTextEdit;
    QWidget *w = new QWidget;
    QPushButton *newSript = new QPushButton("New");
    QPushButton *loadScript = new QPushButton("Load");
    QPushButton *saveScript = new QPushButton("Save");
    QPushButton *clearScript = new QPushButton("Clear");
    QPushButton *executeScript = new QPushButton("Execute");

    // layouts
    QHBoxLayout *hBox = new QHBoxLayout;
    QVBoxLayout *vBox = new QVBoxLayout;

    hBox->addWidget(newSript);
    hBox->addWidget(loadScript);
    hBox->addWidget(saveScript);
    hBox->addWidget(clearScript);
    hBox->addWidget(executeScript);

    vBox->addLayout(hBox);
    vBox->addWidget(tEdit);

    w->setLayout(vBox);
    setWidget(w);
}
