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

class highlighter : public QSyntaxHighlighter
{
  Q_OBJECT
  
public:
  enum Construct {
    VariableDec,
    Loop,
    Comment,
    Boolean,
    Char,
    String,
    Preprocessor,
    LastConstruct = Preprocessor
  };
  
  highlighter(QTextDocument *document);
  
  void setFormatFor(Construct construct,
		    const QTextCharFormat &format);
  QTextCharFormat formatFor(Construct construct) const
  { return m_formats[construct]; }
  
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
  QTextCharFormat m_formats[LastConstruct + 1];
  bool insideWord(QString str, int start, int len);
};