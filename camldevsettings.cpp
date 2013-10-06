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
  
  this->camlPathL = new QLabel("CaML top-level executable:",this);
  this->stdlibPathL = new QLabel("CaML standard library path:",this);
  this->camlToolkitPathL = new QLabel("CaML toolkit path (optional, for compiling):",this);
  
  mainLayout->addWidget(camlPathL);
  mainLayout->addWidget(camlPathField);
  mainLayout->addWidget(stdlibPathL);
  mainLayout->addWidget(stdlibPathField);
  mainLayout->addWidget(camlToolkitPathL);
  mainLayout->addWidget(camlToolkitPathField);
  
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
  this->close();
}