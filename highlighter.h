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