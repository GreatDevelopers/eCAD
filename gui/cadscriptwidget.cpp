#include "cadscriptwidget.h"

CadScriptWidget::CadScriptWidget()
{
    setWindowTitle("Scripting Console");
    isNew = true;

    // widgets and buttons
    w = new QWidget;
    newScript = new QPushButton(QIcon(":icons/images/document.svg"), "");
    newScript->setToolTip("New");
    loadScript = new QPushButton(QIcon(":icons/images/browser.svg"), "");
    loadScript->setToolTip("Load");
    saveScript = new QPushButton(QIcon(":icons/images/task.svg"), "");
    saveScript->setToolTip("Save");
    clearScript = new QPushButton(QIcon(":icons/images/loop-circular.svg"), "");
    clearScript->setToolTip("Clear");
    executeScript = new QPushButton(QIcon(":icons/images/play-circle.svg"), "");
    executeScript->setToolTip("Execute");

    // layouts
    hBox = new QHBoxLayout;
    vBox = new QVBoxLayout;
    verticalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum,
                                     QSizePolicy::Expanding);
    hBox->addWidget(newScript);
    hBox->addWidget(loadScript);
    hBox->addWidget(saveScript);
    hBox->addWidget(clearScript);
    hBox->addWidget(executeScript);

    vBox->addLayout(hBox);
    vBox->addItem(verticalSpacer);

    w->setLayout(vBox);
    setWidget(w);

    jsEngine = new QJSEngine;

    connect(newScript, SIGNAL(pressed()),
            this, SLOT(newScriptFxn()));
    connect(loadScript, SIGNAL(pressed()),
            this, SLOT(loadScriptFxn()));
    connect(saveScript, SIGNAL(pressed()),
            this, SLOT(saveAsScriptFxn()));
    connect(clearScript, SIGNAL(pressed()),
            this, SLOT(clearScriptFxn()));
    connect(executeScript, SIGNAL(pressed()),
            this, SLOT(executeScriptFxn()));

    toggleButtons(0);
    setupJSEngine();
}

void CadScriptWidget::toggleButtons(bool b)
{
    saveScript->setEnabled(b);
    clearScript->setEnabled(b);
    executeScript->setEnabled(b);
}

void CadScriptWidget::setupJSEngine()
{
    QJSValue cadContext = jsEngine->newQObject(this);
    jsEngine->globalObject().setProperty("cad", cadContext);
}

void CadScriptWidget::newScriptFxn()
{
    // creates a new script file
    if (!tEdit)
    {
        tEdit = new QTextEdit;

        if (isNew)
        {
            saveAsScriptFxn();
            isNew = false;
        }

        vBox->removeItem(verticalSpacer);
        vBox->addWidget(tEdit);
    }

    else
    {
        tEdit->clear();
    }

    w->setLayout(vBox);
    setWidget(w);
    toggleButtons(1);

    // sets timer for 60 seconds to auto save
    QTimer *timer = new QTimer(this);
    timer->setInterval(60000);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(autoSave()));
    timer->start();
}

void CadScriptWidget::loadScriptFxn()
{
    // open script file dialog box
    QString fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        isNew = false;
        newScriptFxn();

        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("Load Error"),
                                 tr("Cannot Read Script %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }

        QTextStream in(&file);
        tEdit->setPlainText(in.readAll());
        setCurrentFile(fileName);
    }
}

void CadScriptWidget::autoSave()
{
    // calls saveScript function
    saveScriptFxn(currentFile);
}

bool CadScriptWidget::saveScriptFxn(const QString &fileName)
{
    // writes text stream to file
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Write Error"),
                             tr("Cannot write Script file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out << tEdit->toPlainText();
    setCurrentFile(fileName);
    return true;
}

bool CadScriptWidget::saveAsScriptFxn()
{
    // save script file dialog box
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.exec();
    QStringList files = dialog.selectedFiles();

    if (files.isEmpty())
        return false;

    return saveScriptFxn(files.at(0));
}

void CadScriptWidget::setCurrentFile(const QString &fileName)
{
    // sets the currentFile
    currentFile = fileName;
    tEdit->document()->setModified(false);
    setWindowModified(false);
    QString shownName = currentFile;

    if (currentFile.isEmpty())
        shownName = "Untitled.js";

    setWindowFilePath(shownName);
}

void CadScriptWidget::clearScriptFxn()
{
    // clears the text edit widget
    tEdit->clear();
}

void CadScriptWidget::executeScriptFxn()
{
    // executes the script
    QString text = tEdit->toPlainText();
    QJSValue result = jsEngine->evaluate(text);
}

void CadScriptWidget::drawPoint(int x, int y)
{
    qDebug() << x << "," << y;
}
