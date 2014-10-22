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
  this->setWindowTitle("LemonCaml settings");
  this->settings = set;
#ifdef WIN32
  QString camlPath = settings->value("General/camlPath","./caml/CamlLightToplevel.exe").toString();
#else
  QString camlPath = settings->value("General/camlPath","./caml/CamlLightToplevel").toString();
#endif
  QString stdlibPath = settings->value("General/stdlibPath","./caml/lib/").toString();
  QString kwfilePath = settings->value("General/keywordspath", "./keywords").toString();
  QString treeModelsPath = settings->value("General/treeModelsPath","./graphtree/").toString();
  bool drawTrees = (settings->value("General/drawTrees",0).toInt() == 1)?true:false;
  
  this->mainLayout = new QVBoxLayout;
  this->tabWidget = new QTabWidget;
    
  /* GENERAL TAB */
  this->generalTab = new QWidget;
  QVBoxLayout *generalTabLayout = new QVBoxLayout();
  
  this->camlPathField = new QLineEdit(camlPath,this);
  this->camlPathField->setWhatsThis("This is the path to the Caml toplevel executable.<br /> \
  It should be an absolute path if LemonCaml gets opened from another directory than \
  the place it got compiled in...");
  this->stdlibPathField = new QLineEdit(stdlibPath,this);
  this->stdlibPathField->setWhatsThis("This is the path to the Caml library, a set of compiled .zi and .zo files.<br /> \
  It should be an absolute path if LemonCaml gets opened from another directory than \
  the place it got compiled in...");
  this->keywordsPathField = new QLineEdit(kwfilePath, this);
  this->keywordsPathField->setWhatsThis("This is the path to the \"keywords\" file, which is typically located in the same directory as LemonCaml.<br /> \
  It should be an absolute path if LemonCaml gets opened from another directory than \
  the place it got compiled in...");
  this->numberField = new QSpinBox(this);
  this->numberField->setWhatsThis("This counts how many recent files are to be kept in memory in the \"File->Recent files\" menu.");
  numberField->setRange(0, 20);
  numberField->setSingleStep(1);
  numberField->setValue(settings->value("Recent/number",5).toInt());
  this->treeModelsPathField = new QLineEdit(treeModelsPath, this);
  this->treeModelsPathField->setWhatsThis("This is the path to the tree models dir, which is typically located in the \"gentree\" subdirectory of LemonCaml\'s installation.<br /> \
  It should be an absolute path if LemonCaml gets opened from another directory than \
  the place it got compiled in...");
  this->acceptTrees = new QCheckBox("Draw graphical trees - beta, read documentation!", this);
  this->acceptTrees->setTristate(false);
  this->acceptTrees->setCheckState((drawTrees)?Qt::Checked:Qt::Unchecked);
  
  
  QLabel *camlPathL = new QLabel("CaML top-level executable:",this);
  QLabel *stdlibPathL = new QLabel("CaML standard library path:",this);
  QLabel *numberL = new QLabel("Number of recent files to be kept:", this);
  QLabel *keywordsPathL = new QLabel("Keywords file (for syntax highlighting):", this);
  QLabel *treeModelsPathL = new QLabel("Tree models path:", this);
  
  generalTabLayout->addWidget(camlPathL);
  generalTabLayout->addWidget(camlPathField);
  generalTabLayout->addWidget(stdlibPathL);
  generalTabLayout->addWidget(stdlibPathField);
  generalTabLayout->addWidget(keywordsPathL);
  generalTabLayout->addWidget(keywordsPathField);
  generalTabLayout->addWidget(acceptTrees);
  generalTabLayout->addWidget(treeModelsPathL);
  generalTabLayout->addWidget(treeModelsPathField);
  generalTabLayout->addWidget(numberL);
  generalTabLayout->addWidget(numberField);
  
  generalTab->setLayout(generalTabLayout);
  tabWidget->addTab(generalTab, "General");
  
  /* COLORS TAB */ //TODO: signals/slots to be able to set them....
  this->colorsTab = new QWidget;
  QVBoxLayout *colorsTabLayout = new QVBoxLayout();
  QStringList colorsToSet;
  colorsToSet << "variableDec" << "loop" << "comment" << "preproc" << "boolean" << "string" << "char" << "builtInType" << "builtInFunction";
  QStringList defaultColors;
  defaultColors << "186,19,155" << "0,163,49" << "181,181,181" << "0,224,49" << "0,75,255" << "255,0,0" << "255,0,0" << "0,21,156" << "0,21,156";
  QStringList helpers;
  helpers << "Variable declarations" << "Loops" << "Comments" << "Preprocessor commands" << "Booleans" << "Strings" << "Characters" << "Built in types" << "Built in functions";
  
  for(int i = 0; i < colorsToSet.count(); i++)
  {
     QString curColor = settings->value("Colors/" + colorsToSet[i], defaultColors[i]).toString();
     int* colors = colorFromString(curColor);
     if(colors != NULL)
     {
        //add a BUTTON
        QHBoxLayout *colLayout = new QHBoxLayout;
        colLayout->addWidget(new QLabel(helpers[i], this));
        QPalette btnPalette;
        btnPalette.setColor(QPalette::Button, QColor(colors[0], colors[1], colors[2]));
        QPushButton *btn = new QPushButton("Set...");
        btn->setPalette(btnPalette);
        colLayout->addWidget(btn);
        colorsTabLayout->addLayout(colLayout);
     }
  }
  
  colorsTab->setLayout(colorsTabLayout);
  tabWidget->addTab(colorsTab, "Colors");
  
  mainLayout->addWidget(tabWidget);

  /* OK/CANCEL BUTTONS */
  
  this->OKCancelLayout = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(OKCancelLayout,SIGNAL(accepted()),this,SLOT(saveSettings()));
  connect(OKCancelLayout, SIGNAL(rejected()), this, SLOT(close()));
  
  mainLayout->addWidget(OKCancelLayout);
  
  this->setLayout(mainLayout);
}

void CamlDevSettings::saveSettings()
{
  settings->setValue("General/camlPath", camlPathField->text());
  settings->setValue("General/stdlibPath", stdlibPathField->text());
  settings->setValue("Recent/number", numberField->value());
  settings->setValue("General/keywordspath", keywordsPathField->text());
  settings->setValue("General/drawTrees", (acceptTrees->checkState() == Qt::Checked)?1:0);
  settings->setValue("General/treeModelsPath", treeModelsPathField->text());
  this->close();
}

