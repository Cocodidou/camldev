// commmon.cpp - Common functions
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

#include "common.h"

int* colorFromString(QString str)
{
   QStringList spl = str.split(",");
   if(spl.count() != 3) return NULL;
   bool ok = true;
   int r = spl[0].toInt(&ok, 10);
   int g = spl[1].toInt(&ok, 10);
   int b = spl[2].toInt(&ok, 10);
   if(!ok) return NULL;
   int* ret = new int[3];
   ret[0] = r;
   ret[1] = g;
   ret[2] = b;
   
   return ret;
}

QString removeComments(QString baseStr)
{
   int pos_l = 0;
   int pos_r = 0;
   QString str = baseStr;
   while(str.indexOf("(*") != -1 || str.indexOf("*)") != -1)
   {
      pos_l = str.indexOf("(*");
      if(pos_l < 0) pos_l = 0;
      //pos_r = str.length() - (str.indexOf("*)") + 2);
      pos_r = str.indexOf("*)", pos_l); //looking for a closing comment
      if(pos_r >= 0) pos_r += 2;
      else pos_r = str.length() - 1;
      QString tmpstr = str.left(pos_l) + str.mid(pos_r);
      str = tmpstr;
      //qDebug() << pos_l << " and " << pos_r << "for  length " << str.length();
      //if(str.length() > 100) break;
   }
   return str;
}

QString removeUnusedLineBreaks(QString str, bool isPersonalOutput)
{
   if(isPersonalOutput)
   {
      while(str.startsWith("\n"))
      {
         str = str.right(str.length()-1);
      }
   }
   while(str.indexOf(" \n") != -1 && str.indexOf("\n\n") != -1)
   {
      str = str.replace(" \n","\n",Qt::CaseInsensitive);
      str = str.replace("\n\n","\n",Qt::CaseInsensitive);
   }
   return str;
}

QStringList parseBlockCommand(QString cmd)
{
   QStringList blocks;
   int pos = 0;
   int start = 0;
   bool escapeSequence = false;
   bool inString = false;
   while (pos < cmd.length())
   {
      if(escapeSequence)
      {
         escapeSequence = false;
         QChar c = cmd[pos];
         switch(c.toLatin1())
         {
            case 'n':
               cmd[pos] = '\n';
               break;
            case '"':
               cmd[pos] = '"';
               break;
            case '\\':
               cmd[pos] = '\\';
               break;
            
         }
         cmd.remove(pos - 1, 1);
         pos++;
      }
      else
      {
         if(cmd.at(pos) == '"' )
         {
            inString = (!inString);
         }
         else if(cmd.at(pos) == '\\')
         {
            escapeSequence = true;
         }
         else if(cmd.at(pos) == ' ' && !inString)
         {
            QString toInsert = cmd.mid(start, pos - start); //don't account the space
            if(toInsert.startsWith("\"") && toInsert.endsWith("\""))
            {
               int len = toInsert.length() - 1;
               toInsert = toInsert.left(len).mid(1);
            }
            blocks << toInsert; 
            start = pos + 1;
         }

      }
      pos++;
   }
   
   //the last block
   
   QString toInsert = cmd.mid(start);
   if(toInsert.startsWith("\"") && toInsert.endsWith("\""))
   {
      int len = toInsert.length() - 1;
      toInsert = toInsert.left(len).mid(1);
   }
   
   blocks << toInsert; 
   
   return blocks;
}

int findNextDoubleCommaDot(int pos, QString str)
{
   int i = pos;
   return i;
}

