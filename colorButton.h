#include <QPushButton>
#include <QString>
#include <QPalette>

class colorButton : public QPushButton
{
  Q_OBJECT
public:
  colorButton(const QString &hlp, QWidget *parent): QPushButton(hlp, parent) {
/*      this->colorName = "";
      this->r = 0;
      this->g = 0;
      this->b = 0;
      this->helper = *hlp;
 */     
  };
  
  void setAssociatedColor(QString name, int rc, int gc, int bc, QString hlp) {
    this->colorName = name;
    this->helper = hlp;
    this->r = rc;
    this->b = bc;
    this->g = gc;
    QPalette btnPalette;
    btnPalette.setColor(QPalette::Button, QColor(r, g, b));
    this->setPalette(btnPalette);
  }
  
  void updateColor(QColor clr) {
    QPalette btnPalette;
    btnPalette.setColor(QPalette::Button, clr);
    this->setPalette(btnPalette);
  }
  
  void getColor(int* red, int* green, int* blue) {
    *red = r;
    *green = g;
    *blue = b;
  }
  
  QString getColorInfo() {
      return colorName;
  }
  
private:
  QString colorName;
  int r;
  int g;
  int b;
  QString helper;
};
