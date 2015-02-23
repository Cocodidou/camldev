// highlighter.cpp - Caml syntax highlighter
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

#include "highlighter.h"

highlighter::highlighter(QTextDocument *document, QStringList *kw, QSettings *set)
: QSyntaxHighlighter(document)
{
  this->settings = set;

  this->updateColorSettings();
  createKeywordArray(kw);
   
  this->escapeSequence = false;
}

void highlighter::updateColorSettings()
{
  //called after settings are changed or at build-up
  QStringList colorsToSet;
  colorsToSet << "variableDec" << "loop" << "comment" << "preproc" << "boolean" << "string" << "char" << "builtInType" << "builtInFunction";
  QStringList defaultColors;
  defaultColors << "186,19,155" << "0,163,49" << "181,181,181" << "0,224,49" << "0,75,255" << "255,0,0" << "255,0,0" << "0,21,156" << "0,21,156";
  QStringList helpers;
  helpers << "Variable declarations" << "Loops" << "Comments" << "Preprocessor commands" << "Booleans" << "Strings" << "Characters" << "Built in types" << "Built in functions";
  bool bold[] = { false, true, false, false, true, false, false, false, false };
  bool italics[] = { false, false, true, false, false, false, false, false, false };
  
  
  for(int i = 0; i < colorsToSet.count(); i++)
  {
     QString curColor = settings->value("Colors/" + colorsToSet[i], defaultColors[i]).toString();
     int* colors = colorFromString(curColor);
     if(colors != NULL)
     {
        QTextCharFormat charFormat;
        charFormat.setForeground(QColor(colors[0], colors[1], colors[2]));
        if(bold[i]) charFormat.setFontWeight(QFont::Bold);
        if(italics[i]) charFormat.setFontItalic(true);
        setFormatFor((Construct)i, charFormat);
     }
     delete[] colors;
  } 
}

void highlighter::setFormatFor(Construct construct, const QTextCharFormat &format)
{
   m_formats[construct] = format;
   rehighlight();
}

void highlighter::highlightBlock(const QString &text)
{
   int state = previousBlockState();
   int len = text.length();
   int start = 0;
   int pos = 0;
   
   foreach (const HighlightingRule &rule, highlightingRules) {
      QRegExp expression(rule.pattern);
      int index = expression.indexIn(text);
      while (index >= 0) {
         int length = expression.matchedLength();
         QTextCharFormat fmt = m_formats[rule.format];
         setFormat(index, length, fmt);
         index = expression.indexIn(text, index + length);
      }
   }
   
   while (pos < len) {
      switch (state) {
         case NormalState:
         default:
            while (pos < len) {
               if (text.mid(pos, 2) == "(*") {
                  state = InComment;
                  break;
               } else if (text.at(pos) == '"') {
                  state = InString;
                  setFormat(pos, 1, m_formats[String]);
                  ++pos;
                  break;
               } else if (text.at(pos) == '#') {
                  state = InPreprocessor;
                  setFormat(pos, 1, m_formats[Preprocessor]);
                  ++pos;
                  break;
               } else if (text.at(pos) == '`') {
                  state = InChar;
                  setFormat(pos, 1, m_formats[Char]);
                  ++pos;
                  break;
               } else {
                  //keywords are better handled elsewhere
                  ++pos;

               }
            }
            break;
         case InComment:
            start = pos;
            while (pos < len) {
               if (text.mid(pos, 2) == "*)") {
                  pos += 2;
                  state = NormalState;
                  break;
               } else {
                  ++pos;
               }
            }
            setFormat(start, pos - start,
                      m_formats[Comment]);
            break;
         case InString:
            start = pos;
            while (pos < len) {
               if(escapeSequence)
               {
                  escapeSequence = false;
                  ++pos;
               }
               else
               {
                  if (text.at(pos) == '"') {
                     pos += 1;
                     state = NormalState;
                     break;
                  } else if(text.at(pos) == '\\') {
                     escapeSequence = true;
                     ++pos;
                  } else {
                     ++pos;
                  }
               }
            }
            setFormat(start, pos - start,
                      m_formats[String]);
            break;	
         case InChar:
            start = pos;
            while (pos < len) {
               if(escapeSequence)
               {
                  escapeSequence = false;
                  ++pos;
               }
               else
               {
                  if (text.at(pos) == '`') {
                     pos += 1;
                     state = NormalState;
                     break;
                  } else if(text.at(pos) == '\\') {
                     escapeSequence = true;
                     ++pos;
                  } else {
                     ++pos;
                  }
               }
            }
            setFormat(start, pos - start,
                      m_formats[Char]);
            break;
         case InPreprocessor:
            start = pos;
            while (pos < len) {
               if (text.mid(pos,2) == ";;") {
                  pos += 2;
                  state = NormalState;
                  break;
               } else {
                  ++pos;
               }
            }
            setFormat(start, pos - start,
                      m_formats[Preprocessor]);
            break;
      }
   }
   
   setCurrentBlockState(state);
   
}

bool highlighter::insideWord(QString str, int start, int len)
{
   if(start > 0) {
      char a = str.at(start-1).toLatin1();
      if( ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a == '_' || a == '-' || (a >= '0' && a <= '9')) && (a != ' '))
      {
         return true;
      }
   }
   if(start + len <= str.length()) {
      char a = str.at(start + len).toLatin1();
      if( ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a == '_' || a == '-' || (a >= '0' && a <= '9')) && (a != ' '))
      {
         return true;
      }
   }
   return false;
}

void highlighter::createKeywordArray(QStringList *lst)
{
   int len = lst->count();
   numKW = len;
   
   HighlightingRule rule;
   
   for(int i = 0; i < len; i++)
   {
      if(lst->at(i).at(0) == '#') //comment
         continue;
      
      QStringList spl = lst->at(i).split(";", QString::SkipEmptyParts);
      if(spl.count() == 2)
      {
         //handle special chars in regexps 
         QString cleanPattern = QRegExp::escape(spl[0]);
         
         bool hasLetters = false;
         QRegExp keywordWithLetters("[a-zA-z]");
         if(keywordWithLetters.indexIn(spl[0]) >= 0)
            hasLetters = true;
         
         QString pattern = (hasLetters ? "\\b" : "") + cleanPattern + (hasLetters ? "\\b" : "");
         rule.pattern = QRegExp(pattern);
         rule.format =  static_cast<Construct>(spl[1].toInt());
         highlightingRules.append(rule);
      }

   }
   
}