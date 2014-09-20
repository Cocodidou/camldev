#include "highlighter.h"

highlighter::highlighter(QTextDocument *document)
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
	  } else if (text.mid(pos, 3) == "let" && !insideWord(text, pos, 3)) {
	    setFormat(pos, 3, m_formats[VariableDec]);
	    pos += 3;
	    break;
	  } else if (text.mid(pos, 3) == "rec" && !insideWord(text, pos, 3)) {
       setFormat(pos, 3, m_formats[VariableDec]);
       pos += 3;
       break;
     } else if (text.mid(pos, 9) == "exception" && !insideWord(text, pos, 9)) {
       setFormat(pos, 9, m_formats[VariableDec]);
       pos += 3;
       break;
     } else if (text.mid(pos, 2) == "in" && !insideWord(text, pos, 2)) {
	    setFormat(pos, 2, m_formats[VariableDec]);
	    pos += 2;
	    break;
	  } else if (text.mid(pos, 3) == "ref" && !insideWord(text, pos, 3)) {
	    setFormat(pos, 3, m_formats[VariableDec]);
	    pos += 3;
	    break;
	  } else if ( (text.mid(pos, 2) == "<-" || text.mid(pos, 2) == "->") && !insideWord(text, pos, 2)) {
	    setFormat(pos, 2, m_formats[VariableDec]);
	    pos += 2;
	    break;
	  } else if (text.mid(pos, 4) == "type" && !insideWord(text, pos, 4)) {
       setFormat(pos, 4, m_formats[VariableDec]);
       pos += 4;
       break;
     } else if (text.mid(pos, 4) == "true" && !insideWord(text, pos, 4)) {
	    setFormat(pos,  4, m_formats[Boolean]);
	    pos += 4;
	    break;
	  } else if (text.mid(pos, 5) == "false" && !insideWord(text, pos, 5)) {
	    setFormat(pos,  5, m_formats[Boolean]);
	    pos += 5;
	    break;
	  } else if (text.mid(pos, 5) == "while" && !insideWord(text, pos, 5)) {
	    setFormat(pos,  5, m_formats[Loop]);
	    pos += 5;
	    break;
	  }  else if (text.mid(pos, 3) == "for" && !insideWord(text, pos, 3)) {
	    setFormat(pos,  3, m_formats[Loop]);
	    pos += 3;
	    break;
	  } else if (text.mid(pos, 3) == "try" && !insideWord(text, pos, 3)) {
       setFormat(pos,  3, m_formats[Loop]);
       pos += 3;
       break;
     } else if (text.mid(pos, 4) == "with" && !insideWord(text, pos, 4)) {
       setFormat(pos,  4, m_formats[Loop]);
       pos += 3;
       break;
     } else if (text.mid(pos, 2) == "do" && !insideWord(text, pos, 2)) {
	    setFormat(pos,  2, m_formats[Loop]);
	    pos += 2;
	    break;
	  } else if (text.mid(pos, 4) == "done" && !insideWord(text, pos, 4)) {
	    setFormat(pos,  4, m_formats[Loop]);
	    pos += 4;
	    break;
	  } else if (text.mid(pos, 5) == "match" && !insideWord(text, pos, 5)) {
	    setFormat(pos,  5, m_formats[Loop]);
	    pos += 5;
	    break;
	  } else if (text.mid(pos, 3) == "and" && !insideWord(text, pos, 3)) {
	    setFormat(pos,  3, m_formats[VariableDec]);
	    pos += 3;
	    break;
	  } else if (text.at(pos) == '&' && !insideWord(text, pos, 1)) {
	    setFormat(pos,  1, m_formats[Boolean]);
	    ++pos;
	    break;
	  } else if (text.mid(pos, 2) == "||" && !insideWord(text, pos, 2)) {
	    setFormat(pos,  2, m_formats[Boolean]);
	    pos += 2;
	    break;
	  } else if (text.mid(pos, 4) == "with" && !insideWord(text, pos, 4)) {
	    setFormat(pos,  4, m_formats[Loop]);
	    pos += 4;
	    break;
	  }  else if (text.mid(pos, 2) == "to" && !insideWord(text, pos, 2)) {
	    setFormat(pos,  2, m_formats[Loop]);
	    pos += 2;
	    break;
	  } else if (text.mid(pos, 2) == "if" && !insideWord(text, pos, 2)) {
	    setFormat(pos,  2, m_formats[Loop]);
	    pos += 2;
	    break;
	  } else if (text.mid(pos, 4) == "then" && !insideWord(text, pos, 4)) {
	    setFormat(pos, 4, m_formats[Loop]);
	    pos += 4;
	    break;
	  } else if (text.mid(pos, 4) == "else" && !insideWord(text, pos, 4)) {
	    setFormat(pos, 4, m_formats[Loop]);
	    pos += 4;
	    break;
	  } else {
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
	  if (text.at(pos) == '"') {
	    pos += 1;
	    state = NormalState;
	    break;
	  } else {
	    ++pos;
	  }
	}
	setFormat(start, pos - start,
		  m_formats[String]);
	break;	
      case InChar:
	start = pos;
	while (pos < len) {
	  if (text.at(pos) == '`') {
	    pos += 1;
	    state = NormalState;
	    break;
	  } else {
	    ++pos;
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
    if( ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z')) && (a != ' '))
    {
      return true;
    }
  }
  if(start + len <= str.length()) {
    char a = str.at(start + len).toLatin1();
    if( ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z')) && (a != ' '))
    {
      return true;
    }
  }
  return false;
}
