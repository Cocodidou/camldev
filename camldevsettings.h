// camldevsettings.h - LemonCaml settings window
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

#ifndef CAMLDEVSETTINGS_H
#define CAMLDEVSETTINGS_H

#include <QDialog>
#include <QtGui>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
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
