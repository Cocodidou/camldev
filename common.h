#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QStringList>
#include <QDebug>

int* colorFromString(QString str);
QString removeComments(QString);
QString removeUnusedLineBreaks(QString, bool isPersonalOutput);
QStringList parseBlockCommand(QString cmd);

#endif