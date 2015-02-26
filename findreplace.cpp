// findreplace.cpp - Find/replace toolbar
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

#include "findreplace.h"

/* The following stuff is very complicated, may seem obfuscated.
 * I don't use regexps everywhere for the simple reason they are
 * more expensive than pure string matchings for simple searches */


findReplace::findReplace(QTextEdit *d, highlighter *h) : 
    QGroupBox (tr("Find/replace"))
{
   this->doc = d;
   this->hlt = h;
   this->findText = new QLineEdit(this);
   this->replaceText = new QLineEdit(this);
   QLabel *findLabel = new QLabel(tr("Find:"), this);
   QLabel *replaceLabel = new QLabel(tr("Replace:"), this);
   this->isRegExp = new QCheckBox(tr("Use regular expressions"), this);
   this->caseSensitive = new QCheckBox(tr("Case-sensitive"), this);
   this->showHideReplace = new QPushButton(tr("More..."));
   this->showHideReplace->setIcon(QIcon(":/up.png"));
   this->showHideReplace->setCheckable(true);
   this->searchCaseSensitive = false;
   this->searchRegExp = false;
   
   this->isHighlighting = true; //by default highlight all the found results
   
   this->find = new QPushButton(tr("Find next"));
   this->find->setIcon(QIcon(":/next.png"));
   this->hilitAll = new QPushButton(tr("Highlight All"));
   this->hilitAll->setIcon(QIcon(":/uline_search.png"));
   this->hilitAll->setCheckable(true);
   this->hilitAll->setChecked(isHighlighting);
   this->replace = new QPushButton(tr("Replace next"));
   this->replaceAll = new QPushButton(tr("Replace All"));
   this->statusLabel = new QLabel("", this);
   
   QPushButton *closeButton = new QPushButton(tr("Close"), this);
   closeButton->setIcon(QIcon(":/stopcaml.png"));
   connect(closeButton,SIGNAL(clicked()),this,SLOT(hide()));
   
   QVBoxLayout *layout = new QVBoxLayout();
   
   QHBoxLayout *topLayout = new QHBoxLayout();
   QHBoxLayout *bottomLayout = new QHBoxLayout();
   QHBoxLayout *optionsLayout = new QHBoxLayout();
   
   topLayout->addWidget(findLabel);
   topLayout->addWidget(findText);
   topLayout->addWidget(find);
   topLayout->addWidget(hilitAll);

   
   wrapperReplaceWidget = new QWidget();
   
   bottomLayout->addWidget(replaceLabel);
   bottomLayout->addWidget(replaceText);
   bottomLayout->addWidget(replace);
   bottomLayout->addWidget(replaceAll);
   bottomLayout->setContentsMargins(0,0,0,0);
   
   optionsLayout->addWidget(isRegExp);
   optionsLayout->addWidget(caseSensitive);
   optionsLayout->addWidget(showHideReplace);
   optionsLayout->addWidget(closeButton);
   optionsLayout->addWidget(statusLabel);
   optionsLayout->addStretch(1);
   
   wrapperReplaceWidget->setVisible(false);
   wrapperReplaceWidget->setLayout(bottomLayout);
   wrapperReplaceWidget->setContentsMargins(0,0,0,0);
   
   layout->addLayout(topLayout);
   layout->addWidget(wrapperReplaceWidget);
   layout->addLayout(optionsLayout);
   
   connect(showHideReplace,SIGNAL(clicked(bool)),this,SLOT(triggerReplace(bool)));
   connect(findText,SIGNAL(textChanged(QString)),this, SLOT(doFind(QString)));
   connect(find,SIGNAL(clicked()),this,SLOT(findNextOccurence()));
   connect(replace,SIGNAL(clicked()),this,SLOT(doReplace()));
   connect(replaceAll,SIGNAL(clicked()),this,SLOT(doReplaceAll()));
   connect(caseSensitive,SIGNAL(clicked(bool)),this,SLOT(setCaseSensitive(bool)));
   connect(hilitAll,SIGNAL(clicked(bool)),this,SLOT(triggerHighlightResults(bool)));
   connect(isRegExp,SIGNAL(clicked(bool)),this,SLOT(setRegExp(bool)));
   connect(findText,SIGNAL(returnPressed()),this,SLOT(findNextOccurence()));
   connect(replaceText,SIGNAL(returnPressed()),this,SLOT(doReplace()));
   
   this->setLayout(layout);
}

void findReplace::triggerReplace(bool show)
{
   wrapperReplaceWidget->setVisible(show);
   if(show)
   {
      showHideReplace->setText(tr("Less..."));
      showHideReplace->setIcon(QIcon(":/down.png"));
   }
   else
   {
      showHideReplace->setText(tr("More..."));
      showHideReplace->setIcon(QIcon(":/up.png"));
   }
}

void findReplace::takeFocus()
{
   findText->setFocus(Qt::PopupFocusReason);
}

void findReplace::doFind(QString txt)
{   
   highlightAllResults(); //do it before the check so that the rule may be cleared
   QTextCursor cursor = doc->textCursor();
   if(txt == "") {
      cursor.clearSelection();
      doc->setTextCursor(cursor);
      return;
   }
   QString contents = doc->toPlainText();

   
   Qt::CaseSensitivity sensitivity = (searchCaseSensitive)?Qt::CaseSensitive:Qt::CaseInsensitive;
   
   //find the closest match from the cursor
   
   int prevMatch = 0;
   int nextMatch = 0;
   int pos = cursor.position();
   int len = 0;
   if(!searchRegExp) {
      prevMatch = contents.lastIndexOf(txt, pos, sensitivity);
      nextMatch = contents.indexOf(txt, pos-1, sensitivity);
      len = txt.length();
   } else {
      QRegExp re(txt, sensitivity);
      prevMatch = re.lastIndexIn(contents, pos);
      nextMatch = re.indexIn(contents, pos);
      len = re.matchedLength();
   }
   int nextPos = 0;
   
   if((prevMatch == -1 && nextMatch >= 0) || ( (prevMatch >= 0 && nextMatch >= 0) && (pos - prevMatch >= nextMatch - pos)))
      nextPos = nextMatch;
   else if ((prevMatch >= 0 && nextMatch == -1) || ( (prevMatch >= 0 && nextMatch >= 0) && (pos - prevMatch < nextMatch - pos)))
      nextPos = prevMatch;
   else {
      status(NotFound);
      cursor.setPosition(pos);
      doc->setTextCursor(cursor);
      return;
   }
   
   status(OK);
   

   
   cursor.setPosition(nextPos);
   
   cursor.setPosition(nextPos + len, QTextCursor::KeepAnchor);
   doc->setTextCursor(cursor);
   
}

void findReplace::findNextOccurence()
{
   QString contents = doc->toPlainText();
   QTextCursor cursor = doc->textCursor();
   
   Qt::CaseSensitivity sensitivity = (searchCaseSensitive)?Qt::CaseSensitive:Qt::CaseInsensitive;

   QString txt = findText->text();
   QRegExp re(txt, sensitivity);
   
   if(txt == "") return;
   //find the closest match from the cursor
   int nextMatch = 0;
   int pos = cursor.position();
   int len = 0;
   if(!searchRegExp) {
      nextMatch = contents.indexOf(txt, pos + txt.length(), sensitivity);
      len = txt.length();
   } else {
      nextMatch = re.indexIn(contents, pos);
      len = re.matchedLength();
   }
   
   if (nextMatch >= 0) {
      status(OK);
      cursor.setPosition(nextMatch);
      cursor.setPosition(nextMatch + len, QTextCursor::KeepAnchor);
   } else { //is there any match at the beginning of the document?
      if(!searchRegExp) {
         nextMatch = contents.indexOf(txt, 0, sensitivity);
      } else {
         nextMatch = re.indexIn(contents, 0);
         len = re.matchedLength();
      }
      if (nextMatch >= 0) {
         status(ReachEndDoc);
         cursor.setPosition(nextMatch);
         cursor.setPosition(nextMatch + len, QTextCursor::KeepAnchor);
      } else status(NotFound);
   }
   
   doc->setTextCursor(cursor);
}

void findReplace::setCaseSensitive(bool is)
{
   this->searchCaseSensitive = is;
   doFind(findText->text());
}

void findReplace::setRegExp(bool is)
{
   this->searchRegExp = is;
   doFind(findText->text());
}

void findReplace::triggerHighlightResults(bool doIt)
{
   if(doIt)
   {
      isHighlighting = true;
      highlightAllResults();
   }
   else
   {
      isHighlighting = false;
      hlt->undoSearchRule(); 
   }
}

void findReplace::highlightAllResults()
{
   if(isHighlighting)
   {
      if(findText->text() == "") {
         hlt->undoSearchRule();
         return;
      }
      Qt::CaseSensitivity sensitivity = (searchCaseSensitive)?Qt::CaseSensitive:Qt::CaseInsensitive;
      //do some stuff with the Highlighter
      QString expr = "";
      if(searchRegExp)
         hlt->addSearchRule(QRegExp(findText->text(), sensitivity));
      else {
         hlt->addSearchRule(QRegExp(findText->text(), sensitivity, QRegExp::FixedString));
      }
   }
   
}

void findReplace::doReplace()
{
   QString contents = doc->toPlainText();
   QTextCursor cursor = doc->textCursor();
   
   Qt::CaseSensitivity sensitivity = (searchCaseSensitive)?Qt::CaseSensitive:Qt::CaseInsensitive;
   QString txt = findText->text();
   QRegExp re(txt, sensitivity);
   if(txt == "") return;
   //find the closest match from the cursor
   int nextMatch = 0;
   int pos = 0;
   int len = 0;
   
   if(!cursor.hasSelection())
      pos = cursor.position();
   else
      pos = cursor.selectionStart();
   
   if(!searchRegExp) {
      nextMatch = contents.indexOf(txt, pos-1, sensitivity);
      len = txt.length();
   } else {
      nextMatch = re.indexIn(contents, pos-1);
      len = re.matchedLength();
   }
   
   if (nextMatch >= 0) {
      status(OK);
      cursor.setPosition(nextMatch);
      cursor.setPosition(nextMatch + len, QTextCursor::KeepAnchor);
   } else { //is there any match at the beginning of the document?
      if(!searchRegExp) {
         nextMatch = contents.indexOf(txt, 0, sensitivity);
      } else {
         nextMatch = re.indexIn(contents, 0);
         len = re.matchedLength();
      }
      if (nextMatch >= 0) {
         status(ReachEndDoc);
         cursor.setPosition(nextMatch);
         cursor.setPosition(nextMatch + len, QTextCursor::KeepAnchor);
      }
      else { //not found anywhere: not replacing anything
         status(NotFound);
         return; 
      }
   }
   
   doc->setTextCursor(cursor);
   doc->insertPlainText(replaceText->text());
   
   /* re-select the text */
   cursor.setPosition(nextMatch);
   cursor.setPosition(nextMatch + replaceText->text().length(), QTextCursor::KeepAnchor);
   doc->setTextCursor(cursor);
   
}

void findReplace::doReplaceAll()
{
   //index them all
   int index = 0;
   Qt::CaseSensitivity sensitivity = (searchCaseSensitive)?Qt::CaseSensitive:Qt::CaseInsensitive;
   QString txt = findText->text();
   QTextCursor cursor = doc->textCursor();
   QString document = doc->toPlainText();
   QRegExp re(txt, sensitivity);
   if(txt == "") return;
   
   int len = 0; //with regexp, we can't predict the length of the match
   do
   {
      if(!searchRegExp) {
         index = document.indexOf(txt, index , sensitivity);
         len = txt.length();
      } else {
         index = re.indexIn(document, index);
         len = re.matchedLength();
      }
      if(index >= 0)
      {
         cursor.setPosition(index);
         cursor.setPosition(index + len, QTextCursor::KeepAnchor);
         doc->setTextCursor(cursor);
         doc->insertPlainText(replaceText->text());
         document = doc->toPlainText();
         index += replaceText->text().length();
      }
   } while(index >= 0);
   
}

void findReplace::hide() 
{
   emit hideRequest(false);
}

void findReplace::status(searchstat us)
{
   switch(us)
   {
      case OK:
         statusLabel->setText("");
         break;
      case NotFound:
         statusLabel->setText(tr("Not found"));
         break;
      case ReachEndDoc:
         statusLabel->setText(tr("Reached end of document, searching from the beginning"));
         break;
      default:
         break;
      
   }
   
}
