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