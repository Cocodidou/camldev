// treeparser.h - Tree parser and drawer
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

#ifndef TREEPARSER_H
#define TREEPARSER_H

#include <QString>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <algorithm>
#include <QPalette>


struct tree {
   QString etiquette;
   int profondeur;
   int nbfils;
   int encombrement;
   struct tree* fils;
   struct tree* suivant;
};

class treeParser {
   
public:
   treeParser();
   QImage* parseTree(QString descriptor);
   
private:
   QStringList subtrees;
   struct tree* parseStringIntoTreeList(QString treelist, int prof, int* sonscount); //from a string to a tree* (should it have a single element...)
   int getTreeListHeight(tree* treelist); //tree height
   int findUntilLastClosingBracket(QString baseStr);
   void getTreeWidth(tree* t, int* arr, int decay); //fill arr with the widths from each row
   void draw(tree* tr, QImage* img, QPainter* pnt, int baseX, int xparent, int yparent); //draw!
   void calculateEncombrement(tree* t, int* encombrementParent); //"sort of" width, that also works for widths of forests...
   QPalette* palette;
   
};

#endif