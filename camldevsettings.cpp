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
  QString camlArgs = settings->value("General/camlArgs", "-stdlib ./caml/lib").toString();
  QString kwfilePath = settings->value("General/keywordspath", "./keywords").toString();
  QString treeModelsPath = settings->value("General/treeModelsPath","./gentree/").toString();
  bool drawTrees = (settings->value("General/drawTrees",0).toInt() == 1)?true:false;
  
  this->mainLayout = new QVBoxLayout;
  this->tabWidget = new QTabWidget;
    
  /* GENERAL TAB */
  this->generalTab = new QWidget;
  QVBoxLayout *generalTabLayout = new QVBoxLayout();
  
  this->camlPathField = new QLineEdit(camlPath,this);
  this->camlPathField->setWhatsThis(tr("This is the path to the Caml toplevel executable.<br /> \
  It should be an absolute path if LemonCaml gets opened from another directory than \
  the place it got compiled in..."));
  this->camlArgsField = new QLineEdit(camlArgs,this);
  this->camlArgsField->setWhatsThis(tr("This field contains the arguments passed to the Caml toplevel.<br /> \
  Usually, this is \"-stdlib\" followed by the path to the Caml library, a set of compiled .zi and .zo files.<br /> \
  It should be an absolute path if LemonCaml gets opened from another directory than \
  the place it got compiled in..."));
  this->keywordsPathField = new QLineEdit(kwfilePath, this);
  this->keywordsPathField->setWhatsThis(tr("This is the path to the \"keywords\" file, which is typically located in the same directory as LemonCaml.<br /> \
  It should be an absolute path if LemonCaml gets opened from another directory than \
  the place it got compiled in..."));
  this->numberField = new QSpinBox(this);
  this->numberField->setWhatsThis(tr("This counts how many recent files are to be kept in memory in the \"File->Recent files\" menu."));
  numberField->setRange(0, 20);
  numberField->setSingleStep(1);
  numberField->setValue(settings->value("Recent/number",5).toInt());
  this->treeModelsPathField = new QLineEdit(treeModelsPath, this);
  this->treeModelsPathField->setWhatsThis(tr("This is the path to the tree models dir, which is typically located in the \"gentree\" subdirectory of LemonCaml\'s installation.<br /> \
  It should be an absolute path if LemonCaml gets opened from another directory than \
  the place it got compiled in..."));
  this->acceptTrees = new QCheckBox(tr("Draw graphical trees - beta, read documentation!"), this);
  this->acceptTrees->setTristate(false);
  this->acceptTrees->setCheckState((drawTrees)?Qt::Checked:Qt::Unchecked);
  
  
  QLabel *camlPathL = new QLabel(tr("CaML top-level executable:"),this);
  QLabel *camlArgsL = new QLabel(tr("CaML arguments:"),this);
  QLabel *numberL = new QLabel(tr("Number of recent files to be kept:"), this);
  QLabel *keywordsPathL = new QLabel(tr("Keywords file (for syntax highlighting):"), this);
  QLabel *treeModelsPathL = new QLabel(tr("Tree models path:"), this);
  
  QPushButton *autoConfButton = new QPushButton(tr("Auto-configure"), this);
  connect(autoConfButton, SIGNAL(clicked()), this, SLOT(autoConfDirs()));
  
  generalTabLayout->addWidget(camlPathL);
  generalTabLayout->addWidget(camlPathField);
  generalTabLayout->addWidget(camlArgsL);
  generalTabLayout->addWidget(camlArgsField);
  generalTabLayout->addWidget(keywordsPathL);
  generalTabLayout->addWidget(keywordsPathField);
  generalTabLayout->addWidget(acceptTrees);
  generalTabLayout->addWidget(treeModelsPathL);
  generalTabLayout->addWidget(treeModelsPathField);
  generalTabLayout->addWidget(numberL);
  generalTabLayout->addWidget(numberField);
  generalTabLayout->addWidget(autoConfButton);
  
  generalTab->setLayout(generalTabLayout);
  tabWidget->addTab(generalTab, tr("General"));
  
  /* COLORS TAB */ //TODO: signals/slots to be able to set them....
  this->colorsTab = new QWidget;
  QVBoxLayout *colorsTabLayout = new QVBoxLayout();
  QStringList colorsToSet;
  colorsToSet << "variableDec" << "loop" << "comment" << "preproc" << "boolean" << "string" << "char" << "builtInType" << "builtInFunction" << "searchResult";
  QStringList defaultColors;
  defaultColors << "186,19,155" << "0,163,49" << "181,181,181" << "0,224,49" << "0,75,255" << "255,0,0" << "255,0,0" << "0,21,156" << "0,21,156" << "255,255,0";
  QStringList helpers;
  helpers << tr("Variable declarations") << tr("Loops") << tr("Comments") << tr("Preprocessor commands") << tr("Booleans") << tr("Strings") << tr("Characters") << tr("Built in types") << tr("Built in functions") << tr("Search results");
  
  for(int i = 0; i < colorsToSet.count(); i++)
  {
     QString curColor = settings->value("Colors/" + colorsToSet[i], defaultColors[i]).toString();
     int* colors = colorFromString(curColor);
     if(colors != NULL)
     {
        //add a BUTTON
        QHBoxLayout *colLayout = new QHBoxLayout;
        colLayout->addWidget(new QLabel(helpers[i], this));
        colorButton *btn = new colorButton(tr("Set..."), this);
	btn->setAssociatedColor(colorsToSet[i], colors[0], colors[1], colors[2], helpers[i]);
	connect(btn, SIGNAL(clicked()), this, SLOT(openColorPicker()));
        colLayout->addWidget(btn);
        colorsTabLayout->addLayout(colLayout);
     }
     delete[] colors;
  }
  
  this->colorBeingChanged = "";
  this->buttonToUpdate = NULL;
  
  colorsTab->setLayout(colorsTabLayout);
  tabWidget->addTab(colorsTab, tr("Colors"));
  
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
  settings->setValue("General/camlArgs", camlArgsField->text());
  settings->setValue("Recent/number", numberField->value());
  settings->setValue("General/keywordspath", keywordsPathField->text());
  settings->setValue("General/drawTrees", (acceptTrees->checkState() == Qt::Checked)?1:0);
  settings->setValue("General/treeModelsPath", treeModelsPathField->text());
  this->close();
}

void CamlDevSettings::autoConfDirs()
{
   QString curPath = QDir::toNativeSeparators(QDir::currentPath() + QDir::separator());
#ifdef WIN32
   QString exe = ".exe";
#else
   QString exe = "";
#endif
   QString camlPath = curPath + "caml" + QDir::separator() + "CamlLightToplevel" + exe;
   if(camlPath.indexOf(' ') != -1)
   {
      camlPath = "\"" + camlPath + "\"";
   }
   this->camlPathField->setText(camlPath);
   this->camlArgsField->setText("-stdlib \"" + curPath + "caml" + QDir::separator() + "lib\"");
   this->keywordsPathField->setText(curPath + "keywords");
   this->treeModelsPathField->setText(curPath + "gentree" + QDir::separator());
   
}

/* BEWARE: the following function is converting a QObject to a colorButton, because it implicitly
 * knows that it IS actually a colorButton. This is a bad practice from the developer: never, ever
 * trust the sender... */

void CamlDevSettings::openColorPicker()
{
  //get sender then open the color picker, then set the required color, etc.
  colorButton* snd = (colorButton*) QObject::sender();
  if(snd == NULL)
      return;
  
  this->buttonToUpdate = snd;
  QColorDialog* dlg = new QColorDialog(this);
  int red, green, blue;
  snd->getColor(&red, &green, &blue);
  
  dlg->setOption(QColorDialog::DontUseNativeDialog, false);
  dlg->setCurrentColor(QColor(red, green, blue));
  
  this->colorBeingChanged = snd->getColorInfo();
  
  connect(dlg, SIGNAL(colorSelected(QColor)), this, SLOT(colorChangeValidated(QColor)));
  dlg->open();
}

void CamlDevSettings::colorChangeValidated(QColor color)
{
    if(this->colorBeingChanged == "" || this->buttonToUpdate == NULL) //erroneous call
        return; 
    
    this->setColor(colorBeingChanged, color);
    buttonToUpdate->updateColor(color);
}

void CamlDevSettings::setColor(QString colorId, QColor color) //is bool useful?
{
    int r, g, b;
    color.getRgb(&r, &g, &b, NULL);
    
    QString colorStr = QString::number(r) + "," + QString::number(g) + "," + QString::number(b);
    settings->setValue("Colors/"+colorId, colorStr);
    
}