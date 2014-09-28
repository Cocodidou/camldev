// camldevsettings.cpp - LemonCaml settings window
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

#include "camldevsettings.h"

CamlDevSettings::CamlDevSettings(QWidget *parent, QSettings *set) :
    QDialog(parent)
{
  this->settings = set;
#ifdef WIN32
  this->camlPath = settings->value("General/camlPath","./caml/CamlLightToplevel.exe").toString();
#else
  this->camlPath = settings->value("General/camlPath","./caml/CamlLightToplevel").toString();
#endif
  this->stdlibPath = settings->value("General/stdlibPath","./caml/lib/").toString();
  this->camlToolkitPath = settings->value("General/camlToolkitPath","./caml/comp/").toString();
  
  this->mainLayout = new QVBoxLayout;
  
  this->camlPathField = new QLineEdit(camlPath,this);
  this->camlToolkitPathField = new QLineEdit(camlToolkitPath,this);
  this->stdlibPathField = new QLineEdit(stdlibPath,this);
  this->numberField = new QSpinBox(this);
  numberField->setRange(0, 20);
  numberField->setSingleStep(1);
  numberField->setValue(settings->value("Recent/number",5).toInt());
  
  this->camlPathL = new QLabel("CaML top-level executable:",this);
  this->stdlibPathL = new QLabel("CaML standard library path:",this);
  this->camlToolkitPathL = new QLabel("CaML toolkit path (optional, for compiling):",this);
  this->numberL = new QLabel("Number of recent files to be kept:", this);
  
  mainLayout->addWidget(camlPathL);
  mainLayout->addWidget(camlPathField);
  mainLayout->addWidget(stdlibPathL);
  mainLayout->addWidget(stdlibPathField);
  mainLayout->addWidget(camlToolkitPathL);
  mainLayout->addWidget(camlToolkitPathField);
  mainLayout->addWidget(numberL);
  mainLayout->addWidget(numberField);
  
  this->buttonsLayout = new QHBoxLayout;
  this->ok = new QPushButton("OK",this);
  connect(this->ok,SIGNAL(clicked()),this,SLOT(saveSettings()));
  this->cancel = new QPushButton("Cancel",this);
  connect(this->cancel, SIGNAL(clicked()), this, SLOT(close()));
  //buttonsLayout->addWidget(ok);
  //buttonsLayout->addWidget(cancel);
  
  //mainLayout->addWidget(buttonsLayout);
  mainLayout->addWidget(ok);
  mainLayout->addWidget(cancel);
  this->setLayout(mainLayout);
}

void CamlDevSettings::saveSettings()
{
  settings->setValue("General/camlPath", camlPathField->text());
  settings->setValue("General/stdlibPath", stdlibPathField->text());
  settings->setValue("General/camlToolkitPath", camlToolkitPathField->text());
  settings->setValue("Recent/number", numberField->value());
  this->close();
}