#-------------------------------------------------
#
# Project created by QtCreator 2013-02-16T14:16:45
#
#-------------------------------------------------

QT       += core \
    widgets \
    printsupport

TARGET = lemoncaml

TEMPLATE = app


SOURCES += main.cpp \
    camldevwindow.cpp \
    camldevsettings.cpp \
    inputzone.cpp \
    highlighter.cpp \
    treeparser.cpp \
    common.cpp \
    findreplace.cpp

HEADERS += \
    camldevwindow.h \
    camldevsettings.h \
    inputzone.h \
    highlighter.h \
    treeparser.h \
    common.h \
    colorButton.h \
    findreplace.h

RESOURCES += \
    icons.qrc

lemoncaml.files += lemoncaml
lemoncaml.path = /usr/bin/

INSTALLS += lemoncaml

RC_FILE = progicon.rc
