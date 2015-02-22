// highlighter.h - Caml syntax highlighter
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

#include <QSyntaxHighlighter>
#include <QSettings>
#include "common.h"

class highlighter : public QSyntaxHighlighter
{
  Q_OBJECT
  
public:
  enum Construct {
    VariableDec,
    Loop,
    Comment,
    Preprocessor,
    Boolean,
    String,
    Char,
    BuiltInType,
    BuiltInFunction,
    LastConstruct = BuiltInFunction
  };
  
  highlighter(QTextDocument *document, QStringList *kw, QSettings *settings);
  
  void setFormatFor(Construct construct,
		    const QTextCharFormat &format);
  QTextCharFormat formatFor(Construct construct) const
  { return m_formats[construct]; }
  
  struct keyword {
    QString word;
    Construct type;
  };
  
  void updateColorSettings();
  
protected:
  enum State {
    NormalState = -1,
    InComment,
    InString,
    InChar,
    InPreprocessor
  };
  
  void highlightBlock(const QString &text);

  
private:
  bool escapeSequence;
  QTextCharFormat m_formats[LastConstruct + 1];
  bool insideWord(QString str, int start, int len);
  void createKeywordArray(QStringList *lst);
  keyword *keywords;
  int numKW;
  QSettings *settings;
};