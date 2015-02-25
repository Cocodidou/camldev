// commmon.h - Common functions
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

#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QStringList>
#include <QDebug>

enum indentRule {
   Increment,
   Decrement,
   Reset,
   IDLine //increment what follows, decrement the current line if this keyword is the first to appear
};

struct indentKeyword {
   QString pattern;
   indentRule rule;
};

int* colorFromString(QString str);
QString removeComments(QString);
QString removeCommentsConservative(QString);
QString removeUnusedLineBreaks(QString, bool isPersonalOutput);
QStringList parseBlockCommand(QString cmd);
QString indentCode(QString, QVector<indentKeyword>*, bool);
QString removeIndent(QString);
void fillIndentWords(QVector<indentKeyword>*);




#endif