#include <treeparser.h>

treeParser::treeParser()
{
   palette = new QPalette();
}

QImage* treeParser::parseTree(QString descriptor)
{
   //Parse the tree from a Caml output, and call my private functions to return a proper QImage (QPixmap if needed).
   int sc = 0; //root count - will be unused
   struct tree* actTree = parseStringIntoTreeList(descriptor, 0, &sc);
   int treeheight = getTreeListHeight(actTree);
   int treewidth = 0;
   calculateEncombrement(actTree, &treewidth);
   QImage* ret = new QImage(45 * (treewidth) - 14, 60 * (treeheight + 1) - 29, QImage::Format_RGB32); //why minus 29? because of an extra space left for inexistent arrows
   //ret->fill(QColor(255,255,255,0));
   ret->fill(palette->color(QPalette::Normal, QPalette::Base));
   QPainter* pnt = new QPainter(ret);
   draw(actTree, ret, pnt, 0, 0, (-1));
   return ret;
}

int treeParser::findUntilLastClosingBracket(QString baseStr)
{
   //far from being optimal, this function is still useful
   int len = baseStr.length();
   //int i = baseStr.lastIndexOf(']');
   int i = -1;
   bool found = false;
   int ccnt = 0;
   while(!found && i < len)
   {
     i++;
      if(baseStr[i] == '[')
      {
         ccnt++;
      }
      
      if(baseStr[i] == ']')
      {
         if(ccnt == 0)
            found = true;
         else 
            ccnt--;
      }

   }
   return i;
}

struct tree* treeParser::parseStringIntoTreeList(QString treelist, int prof, int* sonscount) //from a string to a tree* (should it have a single element...)
{
   //if we have recordings such as (label[(label[])(label[])]), then we should be able to go ahead...
   if(treelist.length() > 0)
   {
      if(treelist[0] != '('){
         return NULL;
      }
   }
   
   int tli = treelist.indexOf("["); //index of the first opening bracket
   if(tli == -1 || treelist == "" ) return NULL; //there's no list here - not an error!
   
   tree* ret_tree = new tree;
   ret_tree -> profondeur = prof;
   
   //increase sons count
   *sonscount = *sonscount + 1;
   
   QString before = treelist.left(tli + 1);
   QString md = treelist.mid(tli + 1);
   
   if(before != "") // il y a bien une étiquette - la parser
   {
      //temporary... done later on
      ret_tree->etiquette = before.mid(1, before.length() - 2);
   }
   
   //find our sons, and do the same for them
   int interindex = findUntilLastClosingBracket(md);
   
   //this one is a list
   QString inter = md.left(interindex);
   ret_tree->nbfils = 0;
   struct tree* subtree = parseStringIntoTreeList(inter, prof + 1, &(ret_tree->nbfils));
   //qDebug() << "i have" << ret_tree->nbfils << "sons";
   ret_tree->fils = subtree;
   
   //this one is the rest of the current list, PLUS our parenthese
   QString after = md.mid(interindex);
   
   QString afterbrack = after.mid(after.indexOf(')') + 1);
   
   if(afterbrack.length() > 0)
   {
      struct tree* nextree = parseStringIntoTreeList(afterbrack, prof, sonscount);
      ret_tree->suivant = nextree;
   }
   else
      ret_tree->suivant = NULL;
   
   return ret_tree;
}

int treeParser::getTreeListHeight(tree* treelist) //width: for the image
{
   if(treelist == NULL) return -1;
   return std::max(1 + getTreeListHeight(treelist->fils), getTreeListHeight(treelist->suivant));
}


void treeParser::draw(tree* tr, QImage* img, QPainter* pnt, int baseX, int xparent, int yparent)
{
   if(tr == NULL) return;
   int y = 60 * (tr->profondeur);
   int encombrement = 0;
   if(tr->encombrement > 1) encombrement = tr->encombrement;
   int x = 0;
   if(encombrement % 2 == 1 || encombrement == 0)
      x = baseX + (int)(22.5 * (float)(encombrement));
   else
      x = baseX + (int)(22.5 * (float)(encombrement - 1));

   QRectF rectangle(x, y, 30, 30);
   pnt->setPen(QPen(QColor(Qt::blue)));
   //pnt->setBrush(QBrush(QColor(161, 191, 240, 100), Qt::SolidPattern));
   pnt->setBrush(QBrush(palette->color(QPalette::Normal, QPalette::Highlight), Qt::SolidPattern));
   pnt->drawEllipse(rectangle);
   
   //pnt->setPen(QPen(QColor(Qt::black)));
   pnt->setPen(QPen(palette->color(QPalette::Normal, QPalette::HighlightedText)));
   pnt->drawText(x + 13, y + 18, tr->etiquette);
   //pnt->drawText(x + 13, y + 16, QString::number(tr->encombrement));
   
   if(yparent >= 0)
   {
      pnt->setPen(QPen(palette->color(QPalette::Normal, QPalette::Text)));
      pnt->drawLine(x + 15, y , xparent + 15, yparent + 30);
   }

   if(encombrement == 0) encombrement = 1; //we've drawn a node, even though it had no children...
      
   draw(tr->fils, img, pnt, baseX, x, y);
   draw(tr->suivant, img, pnt, baseX + 45 * encombrement, xparent, yparent);
   
}

void treeParser::calculateEncombrement(tree* t, int* encombrementParent)
{
   if(t == NULL) return;
   
   t->encombrement = 0;
   calculateEncombrement(t->fils, &(t->encombrement)); //updates t->encombrement
   *encombrementParent = *encombrementParent + std::max(1, t->encombrement);
   calculateEncombrement(t->suivant, encombrementParent); //updates parent encombrement
   
}

