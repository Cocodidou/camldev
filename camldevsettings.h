#ifndef CAMLDEVSETTINGS_H
#define CAMLDEVSETTINGS_H

#include <QDialog>
#include <QtGui>
#include <QSettings>

class CamlDevSettings : public QDialog
{
    Q_OBJECT
public:
    explicit CamlDevSettings(QWidget *parent = 0, QSettings* set = NULL);
    
    
private:
  QString camlPath;
  QString camlToolkitPath;
  QString stdlibPath;
  QSettings *settings;
  
  QVBoxLayout *mainLayout;
  
  QLineEdit *camlPathField;
  QLineEdit *camlToolkitPathField;
  QLineEdit *stdlibPathField;
  
  QLabel *camlPathL;
  QLabel *camlToolkitPathL;
  QLabel *stdlibPathL;
  
  QHBoxLayout *buttonsLayout;
  QPushButton *ok;
  QPushButton *cancel;
  
signals:
    
public slots:
  void saveSettings();
    
};

#endif // CAMLDEVSETTINGS_H
