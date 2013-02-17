#include "camldevwindow.h"

CamlDevWindow::CamlDevWindow(QWidget *parent) :
    QMainWindow(parent)
{

    this->camlProcess = new QProcess(this);
    this->camlStarted = false;
    this->currentFile = "";
    this->unsavedChanges = false;
    this->programTitle = "CamlDev alpha";
    /* The window title and icon */
    this->setWindowTitle(this->programTitle + " - " + "untitled");
    this->setWindowIcon(QIcon(":/progicon.png"));

    /* The main window elements : two text-areas and a splitter */
    this->split = new QSplitter(Qt::Horizontal,this);
    this->inputZone = new InputZone();
    this->inputZone->setTabStopWidth(20);
    this->inputZone->setAcceptRichText(false);
    this->outputZone = new QTextEdit(this);
    this->outputZone->setReadOnly(true);
    this->outputZone->setTabStopWidth(20);
    this->setCentralWidget(split);


    split->addWidget(this->inputZone);
    split->addWidget(this->outputZone);
    split->showMaximized();

    /* The actions */
    this->actionNew = new QAction("New",this);
    this->actionNew->setIcon(QIcon(":/new.png"));
    this->actionOpen = new QAction("Open",this);
    this->actionOpen->setIcon(QIcon(":/open.png"));
    this->actionSaveAs = new QAction("Save As",this);
    this->actionSaveAs->setIcon(QIcon(":/saveas.png"));
    this->actionSave = new QAction("Save",this);
    this->actionSave->setIcon(QIcon(":/save.png"));
    this->actionAutoIndent = new QAction("Auto-indent code",this);
    this->actionPrint = new QAction("Print",this);
    this->actionPrint->setIcon(QIcon(":/print.png"));
    this->actionClearOutput = new QAction("Clear output",this);
    this->actionQuit = new QAction("Quit",this);
    this->actionQuit->setIcon(QIcon(":/exit.png"));

    this->actionSendCaml = new QAction("Send Code to Caml",this);
    this->actionSendCaml->setIcon(QIcon(":/sendcaml.png"));
    this->actionInterruptCaml = new QAction("Interrupt Caml",this);
    this->actionInterruptCaml->setIcon(QIcon(":/interrupt.png"));
    this->actionStopCaml = new QAction("Stop Caml",this);
    this->actionStopCaml->setIcon(QIcon(":/stopcaml.png"));

    /* The toolbar */
    this->toolbar = new QToolBar("Tools",this);
    this->toolbar->addAction(actionNew);
    this->toolbar->addAction(actionOpen);
    this->toolbar->addAction(actionSave);
    this->toolbar->addAction(actionSaveAs);
    this->toolbar->addAction(actionPrint);
    this->toolbar->addSeparator();
    this->toolbar->addAction(actionSendCaml);
    this->toolbar->addAction(actionInterruptCaml);
    this->toolbar->addAction(actionStopCaml);
    this->addToolBar(this->toolbar);

    /* The menubar */
    this->menuFile = this->menuBar()->addMenu("File");
    this->menuFile->addAction(actionNew);
    this->menuFile->addAction(actionOpen);
    this->menuFile->addAction(actionSave);
    this->menuFile->addAction(actionSaveAs);
    this->menuFile->addAction(actionQuit);

    this->menuEdit = this->menuBar()->addMenu("Edit");
    this->menuEdit->addAction(actionAutoIndent);
    this->menuEdit->addAction(actionClearOutput);

    this->menuCaml = this->menuBar()->addMenu("Caml");
    this->menuCaml->addAction(actionSendCaml);
    this->menuCaml->addAction(actionInterruptCaml);
    this->menuCaml->addAction(actionStopCaml);

    this->menuHelp = this->menuBar()->addMenu("Help");

    /* Connections */
    connect(actionSendCaml,SIGNAL(triggered()),this,SLOT(sendCaml()));
    connect(camlProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(readCaml()));
    connect(camlProcess,SIGNAL(readyReadStandardError()),this,SLOT(readCamlErrors()));
    connect(actionStopCaml,SIGNAL(triggered()),this,SLOT(stopCaml()));
    connect(camlProcess,SIGNAL(started()),this,SLOT(camlOK()));
    connect(actionInterruptCaml,SIGNAL(triggered()),this,SLOT(interruptCaml()));
    connect(inputZone,SIGNAL(controlEnterPressed()),this,SLOT(sendCaml()));
    connect(actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(inputZone,SIGNAL(textChanged()),this,SLOT(textChanged()));
    connect(actionNew,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(actionClearOutput,SIGNAL(triggered()),this->outputZone,SLOT(clear()));
    connect(actionQuit,SIGNAL(triggered()),this,SLOT(close()));
    connect(actionPrint,SIGNAL(triggered()),this,SLOT(print()));
    this->startCamlProcess();
}

CamlDevWindow::~CamlDevWindow()
{
    camlProcess->close();

}

bool CamlDevWindow::startCamlProcess()
{
    /* Start the Caml process */
    camlProcess->setWorkingDirectory("./caml/");
    camlProcess->start("./CamlLightToplevel");
    return (camlProcess->state() == QProcess::Starting || camlProcess->state() == QProcess::Running);
}

void CamlDevWindow::sendCaml()
{
    if(!camlStarted)
    {
        camlStarted = startCamlProcess();
        if(!camlStarted)
        {
            QMessageBox::warning(this,"Warning","Unable to start Caml toplevel!!");
        }
    }
    QTextCursor cursor = inputZone->textCursor();
    int curPos = cursor.position();
    QString text = inputZone->toPlainText();
    //QString text = removeComments(text_r);
    int startPos = (text.lastIndexOf(";;",curPos)) + 2;
    if(startPos == 1)
    {
        startPos = 0;
    }

    int endPos = (text.indexOf(";;",curPos)) + 2;
    if(endPos == 1) endPos = text.length();
    if(curPos == text.length()) //avoid sending the whole text
    {
        startPos = (text.lastIndexOf(";;",curPos - 2)) + 2;
        if(startPos == 1) startPos = 0;
    }
    QString toWrite = text.mid(startPos,endPos - startPos)  + "\n\0";
    toWrite = removeComments(toWrite);
    toWrite = removeUnusedLineBreaks(toWrite,true);

    appendOutput(toWrite,Qt::blue);

    camlProcess->write(toWrite.toLocal8Bit());

    int nextCurPos = text.indexOf(";;",curPos) + 2;
    if(nextCurPos == 1){ nextCurPos = text.length();}

    cursor.setPosition(nextCurPos,QTextCursor::MoveAnchor);
    inputZone->setTextCursor(cursor);
}

void CamlDevWindow::readCamlErrors()
{
    QString stdErr = camlProcess->readAllStandardError();
    stdErr = removeUnusedLineBreaks(stdErr,false);
    if(stdErr != "") appendOutput(stdErr,Qt::red);

}

void CamlDevWindow::readCaml()
{

    QString stdOut = camlProcess->readAllStandardOutput();
    stdOut = removeUnusedLineBreaks(stdOut,false);
    if(stdOut != "") appendOutput(stdOut,Qt::black);

}

void CamlDevWindow::camlOK()
{
    this->camlStarted = true;
}

void CamlDevWindow::stopCaml()
{
    this->camlStarted = false;
    camlProcess->close();
    this->outputZone->setTextColor(Qt::black);
    this->outputZone->append("Caml Stopped");

}

void CamlDevWindow::interruptCaml()
{
#ifdef WIN32
    qDebug() << "Not implemented yet";
#else
    if(camlStarted) kill(camlProcess->pid(),SIGINT);
#endif
}

QString CamlDevWindow::removeComments(QString str)
{
    int pos_l = 0;
    int pos_r = 0;
    while(str.indexOf("(*") != -1 || str.indexOf("*)") != -1)
    {
        pos_l = str.indexOf("(*");
        if(pos_l < 0) pos_l = 0;
        pos_r = str.length() - (str.indexOf("*)") + 2);
        if(str.indexOf("*)") < 0) pos_r = 0;
        str = str.left(pos_l) + str.right(pos_r) ;
    }
    return str;
}

QString CamlDevWindow::removeUnusedLineBreaks(QString str, bool isPersonalOutput)
{
    if(isPersonalOutput)
    {
        while(str.startsWith("\n"))
        {
            str = str.right(str.length()-1);
        }
    }
    while(str.indexOf(" \n") != -1 && str.indexOf("\n\n") != -1)
    {
        str = str.replace(" \n","\n",Qt::CaseInsensitive);
        str = str.replace("\n\n","\n",Qt::CaseInsensitive);
    }
    return str;
}

bool CamlDevWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save As...","");
    if(fileName.isEmpty())
    {
        return false;
    }
    currentFile = fileName;
    return saveFile(currentFile);

}

bool CamlDevWindow::save()
{
    if(currentFile.isEmpty()) return saveAs();
    else return saveFile(currentFile);
}

bool CamlDevWindow::saveFile(QString file)
{
    QFile f(file);
    if(!f.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this,"Warning","Unable to save file !!");
        return false;
    }
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString output = codec->fromUnicode(inputZone->toPlainText());
    f.write(output.toAscii());
    f.close();
    this->setWindowTitle(this->programTitle + " - " + f.fileName());
    this->unsavedChanges = false;
    return true;
}

void CamlDevWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open","");
    if(!fileName.isEmpty())
    {
        currentFile = fileName;
        openFile(currentFile);
    }

}

void CamlDevWindow::openFile(QString file)
{
    QFile f(file);
    if(!f.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,"Warning","Unable to open file !!");
        return;
    }
    this->newFile(); //clear everything
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    inputZone->clear();
    inputZone->append(codec->toUnicode(f.readAll()));
    this->setWindowTitle(this->programTitle + " - " + f.fileName());
    this->unsavedChanges = false;
    QTextCursor cursor = inputZone->textCursor();
    cursor.setPosition(0,QTextCursor::MoveAnchor);
    inputZone->setTextCursor(cursor);

}

void CamlDevWindow::appendOutput(QString str, QColor color)
{
    QTextCursor tc = outputZone->textCursor();
    tc.setPosition(outputZone->toPlainText().length());
    outputZone->setTextCursor(tc);
    outputZone->setTextColor(color);
    outputZone->insertPlainText(str);
    tc.setPosition(outputZone->toPlainText().length());
    outputZone->setTextCursor(tc);
}

void CamlDevWindow::textChanged()
{
    if(!this->unsavedChanges)
    {
        this->unsavedChanges = true;
        this->setWindowTitle(this->windowTitle() + " (*)");
    }
    this->syntaxingColoration();
}

void CamlDevWindow::newFile()
{
    if(!exitCurrentFile()) return;
    this->stopCaml();
    this->inputZone->clear();
    this->unsavedChanges = false;
    this->currentFile = "";

    this->outputZone->clear();
    this->setWindowTitle(this->programTitle + " - " + "untitled");
    while(camlProcess->state() != QProcess::NotRunning)
    {
        camlProcess->close();
    }
    this->startCamlProcess();
}

void CamlDevWindow::syntaxingColoration()
{
    /* Stub. Unless someone finds a way to highlight the syntax without using regexps, vectors and all that, there won't be any. */
}

bool CamlDevWindow::exitCurrentFile()
{
    if(!unsavedChanges) return true;
    else
    {
        int btn = QMessageBox::question(this,"Save changes before closing?","Your changes have not been saved! Would you like to do that now?",QMessageBox::Save,QMessageBox::Discard,QMessageBox::Cancel);
        bool ret = false;
        switch(btn)
        {

        case QMessageBox::Save:
            ret = this->save(); break;
        case QMessageBox::Discard:
            ret = true; break;
        case QMessageBox::Cancel:
            ret = false; break;

        }
        return ret;
    }

}

void CamlDevWindow::closeEvent(QCloseEvent *event)
{
    if(exitCurrentFile()) event->accept();
    else event->ignore();
}

void CamlDevWindow::print()
{
    //Stub.
}
