#include "cadcommandwidget.h"

CadCommandWidget::CadCommandWidget()
{
    // widgets
    QLineEdit *lEdit = new QLineEdit;
    QTextEdit *tEdit = new QTextEdit;
    QWidget *w = new QWidget;

    // labels
    QLabel *lLabel = new QLabel("Enter Command:");
    QLabel *tLabel = new QLabel("Command History:");

    // layouts
    QHBoxLayout *hBox = new QHBoxLayout;
    QVBoxLayout *vBox1 = new QVBoxLayout;
    QVBoxLayout *vBox2 = new QVBoxLayout;

    // setting properties
    tEdit->setReadOnly(true);

    vBox1->addWidget(lLabel);
    vBox1->addWidget(lEdit);
    vBox2->addWidget(tLabel);
    vBox2->addWidget(tEdit);
    hBox->addLayout(vBox1);
    hBox->addLayout(vBox2);

    w->setLayout(hBox);
    setWidget(w);
}
