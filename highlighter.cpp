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

highlighter::highlighter(QTextDocument *document, QStringList *kw)
: QSyntaxHighlighter(document)
{
   QTextCharFormat variableDecFormat;
   variableDecFormat.setForeground(QColor(186, 19, 155));
   // variableDecFormat.setFontWeight(QFont::Bold);
   setFormatFor(VariableDec, variableDecFormat);
   
   QTextCharFormat loopFormat;
   loopFormat.setForeground(QColor(0, 163, 49));
   loopFormat.setFontWeight(QFont::Bold);
   setFormatFor(Loop, loopFormat);
   
   QTextCharFormat commentFormat;
   commentFormat.setForeground(QColor(181, 181, 181));
   commentFormat.setFontItalic(true);
   setFormatFor(Comment, commentFormat);
   
   QTextCharFormat preprocFormat;
   preprocFormat.setForeground(QColor(0, 224, 49));
   preprocFormat.setFontItalic(true);
   setFormatFor(Preprocessor, preprocFormat);
   
   QTextCharFormat booleanFormat;
   booleanFormat.setForeground(QColor(0, 75, 255));
   booleanFormat.setFontWeight(QFont::Bold);
   setFormatFor(Boolean, booleanFormat);
   
   QTextCharFormat stringFormat;
   stringFormat.setForeground(QColor(255, 0, 0));
   setFormatFor(String, stringFormat);
   
   QTextCharFormat charFormat;
   charFormat.setForeground(QColor(255, 0, 0));
   setFormatFor(Char, charFormat);
   
   QTextCharFormat builtInTypeFormat;
   builtInTypeFormat.setForeground(QColor(0, 21, 156));
   setFormatFor(BuiltInType, builtInTypeFormat);
   
   QTextCharFormat builtInFunctionFormat;
   builtInFunctionFormat.setForeground(QColor(0, 21, 156));
   setFormatFor(BuiltInFunction, builtInFunctionFormat);
   
   createKeywordArray(kw);
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
                  bool foundKW = false;
                  int i = 0;
                  while(i < numKW && !foundKW)
                  {
                     int keywlen = keywords[i].word.length();
                     if(text.mid(pos, keywlen) == keywords[i].word && !insideWord(text, pos, keywlen))
                     {
                        setFormat(pos, keywlen, m_formats[keywords[i].type]);
                        pos += keywlen;
                        foundKW = true;
                     }
                     i++;
                  }
                  if(!foundKW)
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
   keywords = new keyword[len];
   for(int i = 0; i < len; i++)
   {
      QStringList spl = lst->at(i).split(";", QString::SkipEmptyParts);
      if(spl.count() == 2)
      {
         keywords[i].word = spl[0];
         keywords[i].type = static_cast<Construct>(spl[1].toInt());
      }
      else //skip
      {
         keywords[i].word = "";
         keywords[i].type = BuiltInType; //any...
      }
   }
   numKW = len;
   
}