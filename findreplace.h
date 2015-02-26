// findreplace.h - Find/replace toolbar
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


#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTextDocument>
#include <QDebug>
#include <QRegExp>
#include "highlighter.h"
#include "inputzone.h"

#ifndef FINDREPLACE_H
#define FINDREPLACE_H



class findReplace : public QGroupBox
{
   Q_OBJECT
public:
   findReplace(QTextEdit*, highlighter*);
   void takeFocus();
   
   enum searchstat {
      OK,
      NotFound,
      ReachEndDoc
   };
   
private:
   QVBoxLayout *layout;
   QLineEdit *findText;
   QLineEdit *replaceText;
   QCheckBox *caseSensitive;
   QPushButton *showHideReplace;
   QCheckBox *isRegExp;
   QPushButton *closeButton;
   QWidget *wrapperReplaceWidget;
   
   QPushButton *find;
   QPushButton *hilitAll;
   QPushButton *replace;
   QPushButton *replaceAll;
   QTextEdit *doc;
   QLabel *statusLabel;
   highlighter *hlt;
   
   bool searchCaseSensitive;
   bool searchRegExp;
   bool isHighlighting;

   void status(searchstat us);
   void highlightAllResults();
   
signals:
   void hideRequest(bool);
   
public slots:
   void triggerReplace(bool show);
   
private slots:
   void doFind(QString);
   void findNextOccurence();
   void setCaseSensitive(bool);
   void setRegExp(bool);
   void triggerHighlightResults(bool);
   void doReplace();
   void doReplaceAll();
   void hide();
};



#endif