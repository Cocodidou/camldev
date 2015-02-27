// camldevwindow.h - LemonCaml main window
// This file is part of LemonCaml - Copyright (C) 2012-2014 Corentin FERRY
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CAMLDEVWINDOW_H
#define CAMLDEVWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QProcess>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrintDialog>
#include <QFontDialog>
#include <QTextEdit>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QTextCodec>
#include <QSplitter>
#include <QPrinter>
#include <QKeySequence>
#include <QVBoxLayout>
#include "treeparser.h"
#include "inputzone.h"
#include "highlighter.h"
#include "camldevsettings.h"
#include "common.h"
#include "findreplace.h"

#ifndef WIN32
#include <unistd.h>
#include <signal.h>
#else
#include <windows.h>
#endif


class CamlDevWindow : public QMainWindow
{
   Q_OBJECT
public:
   explicit CamlDevWindow(QString wd = "", QWidget *parent = 0);
   ~CamlDevWindow();
   void openFile(QString file);
   bool saveFile(QString file);
   bool exitCurrentFile();
   
private:
   void appendOutput(QString str, QColor color);
   QString programTitle;
   bool startCamlProcess();
   bool camlStarted;
   bool unsavedChanges;
   
   QString currentFile;
   InputZone *inputZone;
   QTextEdit *outputZone;
   QSplitter *split;
   QToolBar *toolbar;
   QAction *actionNew;
   QAction *actionOpen;
   QAction *actionSave;
   QAction *actionSaveAs;
   QAction *actionPrint;
   QAction *actionQuit;
   QAction *actionStopCaml;
   QAction *actionInterruptCaml;
   QAction *actionSendCaml;
   QAction *actionShowSettings;
   QAction *actionClearOutput;
   QAction *actionAutoIndent;
   QAction *actionFollowCursor;
   QAction *actionUndo;
   QAction *actionRedo;
   QAction *actionDelete;
   QAction *actionChangeInputFont;
   QAction *actionChangeOutputFont;
   QAction *actionAbout;
   QAction *actionAboutQt;
   QAction *actionHighlightEnable;
   QAction *actionZoomIn;
   QAction *actionZoomOut;
   QAction *actionFind;
   QAction **recent = NULL;
   QString* recentFiles;
   int numRecentFiles;
   QMenu *menuFile;
   QMenu *menuEdit;
   QMenu *menuCaml;
   QMenu *menuHelp;
   QMenu *menuRecent;
   QProcess *camlProcess;
   QSettings *settings;
   QSettings *globalSettings;
   QPrinter *printer;
   highlighter *hilit;
   QString cwd;
   void closeEvent(QCloseEvent *event);
   void resizeEvent(QResizeEvent *event);
   void moveEvent(QMoveEvent * event);
   void populateRecent();
   void generateRecentMenu();
   void updateRecent();
   bool highlightTriggered;
   int graphCount;
   void processSetupPrinter(QStringList *commands);
   void processSubstituteTree(QStringList *commands);
   void processCommandList(QStringList *commands);
   void processRegisterTreeType(QStringList *commands);
   QStringList treevars;
   QStringList treevalues;
   void autoLoadML(QString location);
   bool drawTrees;
   QVector<indentKeyword> indentWords;
   findReplace *find;
   QVBoxLayout *centralBox;
   
signals:
   
public slots:
   void sendCaml();
   void readCaml();
   void stopCaml();
   void camlOK();
   void interruptCaml();
   bool saveAs();
   bool save();
   void open();
   void textChanged();
   void newFile();
   void readCamlErrors();
   void print();
   void changeInputFont();
   void changeOutputFont();
   void doPrint();
   void showSettings();
   void zoomIn();
   void zoomOut();
   void about();
   void aboutQt();
   void openRecent();
   void updateCamlStatus(QProcess::ProcessState newState);
   void toggleHighlightOn(bool doHighlight);
   void toggleAutoIndentOn(bool doIndent);
   void autoIndentCode();
   void handleLineBreak();
   void unindent();
   void triggerFindReplace(bool show);
   
};

#endif // CAMLDEVWINDOW_H
