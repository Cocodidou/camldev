#include "camldevwindow.h"

CamlDevWindow::CamlDevWindow(QWidget *parent) :
    QMainWindow(parent)
{

    this->camlProcess = new QProcess(this);
    this->camlStarted = false;
    this->currentFile = "";
    this->unsavedChanges = false;
    this->programTitle = "LemonCaml";
    /* The window title and icon */
    this->setWindowTitle(this->programTitle + " - " + "untitled");
    this->setWindowIcon(QIcon(":/progicon.png"));

    this->settings = new QSettings("Cocodidou", "LemonCaml");
    
    /* The main window elements : two text-areas and a splitter */
    this->split = new QSplitter(Qt::Horizontal,this);
    this->inputZone = new InputZone();
    this->inputZone->setTabStopWidth(20);
    this->inputZone->setAcceptRichText(false);
    
    QString iFont = settings->value("Input/Font", "").toString();
    QFont inputFont;
    inputFont.fromString(iFont);
    this->inputZone->setFont(inputFont);
    
    this->outputZone = new QTextEdit(this);
    this->outputZone->setReadOnly(true);
    this->outputZone->setTabStopWidth(20);
    
    QString oFont = settings->value("Output/Font", "").toString();
    QFont outputFont;
    outputFont.fromString(oFont);
    this->outputZone->setFont(outputFont);
    
    this->setCentralWidget(split);


    split->addWidget(this->inputZone);
    split->addWidget(this->outputZone);
    split->showMaximized();

    /* the printer*/
    this->printer = new QPrinter(QPrinter::HighResolution);
    
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

    this->actionUndo = new QAction("Undo",this);
    this->actionRedo = new QAction("Redo",this);
    this->actionDelete = new QAction("Delete",this);
    this->actionChangeInputFont = new QAction("Change Input Font",this);
    this->actionChangeOutputFont = new QAction("Change Output Font",this);

    this->actionSendCaml = new QAction("Send Code to Caml",this);
    this->actionSendCaml->setIcon(QIcon(":/sendcaml.png"));
    this->actionInterruptCaml = new QAction("Interrupt Caml",this);
    this->actionInterruptCaml->setIcon(QIcon(":/interrupt.png"));
    this->actionStopCaml = new QAction("Stop Caml",this);
    this->actionStopCaml->setIcon(QIcon(":/stopcaml.png"));

    this->actionAbout = new QAction("Demmerdez-vous",this);

    /* The toolbar */
    this->toolbar = new QToolBar("Tools",this);
    this->toolbar->addAction(actionNew);
    this->toolbar->addAction(actionOpen);
    this->toolbar->addAction(actionSave);
    this->toolbar->addAction(actionSaveAs);
    this->toolbar->addAction(actionPrint);
    this->toolbar->addSeparator();
    this->toolbar->addAction(actionUndo);
    this->toolbar->addAction(actionRedo);
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
    this->menuEdit->addAction(actionUndo);
    this->menuEdit->addAction(actionRedo);
    this->menuEdit->addAction(actionDelete);
    this->menuEdit->addSeparator();
    this->menuEdit->addAction(actionAutoIndent);
    this->menuEdit->addAction(actionClearOutput);
    this->menuEdit->addAction(actionChangeInputFont);
    this->menuEdit->addAction(actionChangeOutputFont);

    this->menuCaml = this->menuBar()->addMenu("Caml");
    this->menuCaml->addAction(actionSendCaml);
    this->menuCaml->addAction(actionInterruptCaml);
    this->menuCaml->addAction(actionStopCaml);

    this->menuHelp = this->menuBar()->addMenu("Help");
    this->menuHelp->addAction(this->actionAbout);

    /* Connections */
    connect(actionSendCaml,SIGNAL(triggered()),this,SLOT(sendCaml()));
    connect(camlProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(readCaml()));
    connect(camlProcess,SIGNAL(readyReadStandardError()),this,SLOT(readCamlErrors()));
    connect(actionStopCaml,SIGNAL(triggered()),this,SLOT(stopCaml()));
    connect(camlProcess,SIGNAL(started()),this,SLOT(camlOK()));
    connect(actionInterruptCaml,SIGNAL(triggered()),this,SLOT(interruptCaml()));
    connect(inputZone,SIGNAL(controlEnterPressed()),this,SLOT(sendCaml()));
    connect(inputZone,SIGNAL(controlSPressed()),this,SLOT(save()));
    connect(inputZone,SIGNAL(controlOPressed()),this,SLOT(open()));
    connect(inputZone,SIGNAL(controlPPressed()),this,SLOT(print()));
    connect(actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(inputZone,SIGNAL(textChanged()),this,SLOT(textChanged()));
    connect(actionNew,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(actionClearOutput,SIGNAL(triggered()),this->outputZone,SLOT(clear()));
    connect(actionChangeInputFont,SIGNAL(triggered()),this,SLOT(changeInputFont()));
    connect(actionChangeOutputFont,SIGNAL(triggered()),this,SLOT(changeOutputFont()));
    connect(actionQuit,SIGNAL(triggered()),this,SLOT(close()));
    connect(actionPrint,SIGNAL(triggered()),this,SLOT(print()));
    connect(actionUndo,SIGNAL(triggered()),this->inputZone,SLOT(undo()));
    connect(actionRedo,SIGNAL(triggered()),this->inputZone,SLOT(redo()));
    connect(actionDelete,SIGNAL(triggered()),this->inputZone,SLOT(paste()));

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
        //currentFile = fileName;
        openFile(fileName);
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
    //QString curFile = currentFile;
    this->newFile(); //clear everything
    currentFile = file; //restore filename
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    inputZone->clear();
    inputZone->setText(codec->toUnicode(f.readAll()));
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
    QPrintDialog dlg(printer, this);
    dlg.open(this, SLOT(doPrint()));
}

void CamlDevWindow::changeInputFont()
{
    this->inputZone->setFont(QFontDialog::getFont(0, inputZone->font()));
    QFont fnt = inputZone->font();
    settings->setValue("Input/Font", fnt.toString());
}

void CamlDevWindow::changeOutputFont()
{
    this->outputZone->setFont(QFontDialog::getFont(0, outputZone->font()));
    QFont fnt = outputZone->font();
    settings->setValue("Output/Font", fnt.toString());
}

void CamlDevWindow::doPrint()
{
    this->inputZone->print(printer); 
}