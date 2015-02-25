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
      pos_r = str.indexOf("*)", pos_l); //looking for a closing comment
      if(pos_r >= 0) pos_r += 2;
      else pos_r = str.length() - 1;
      QString tmpstr = str.left(pos_l) + str.mid(pos_r);
      str = tmpstr;
   }
   return str;
}


QString removeCommentsConservative(QString baseStr) //keeps the line breaks, used for indentation analysis purposes
{
   int pos_l = 0;
   int pos_r = 0;
   QString str = baseStr;
   while(str.indexOf("(*") != -1 || str.indexOf("*)") != -1)
   {
      pos_l = str.indexOf("(*");
      if(pos_l < 0) pos_l = 0;
      pos_r = str.indexOf("*)", pos_l); //looking for a closing comment
      if(pos_r >= 0) pos_r += 2;
      else pos_r = str.length() - 1;

      //count how many \n's in the comment
      int brCount = str.mid(pos_l, pos_r - pos_l).count('\n');

      QString equivCp = "";
      for(int j = 0; j < brCount; j++)
      {
         equivCp += '\n';
      }
      QString tmpstr = str.left(pos_l) + equivCp + str.mid(pos_r);
      str = tmpstr;
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


QString indentCode(QString code, QVector<indentKeyword> *iw)
{
   int indentLevel = 0;
   
   QString cleanCode = removeCommentsConservative(code);
   
   QStringList separatedLines = code.split('\n', QString::KeepEmptyParts);
   QStringList treatedLines = cleanCode.split('\n', QString::KeepEmptyParts);
   
   QStringList result;
   for(int k = 0; k < separatedLines.count(); k++)
   {
      QString line = treatedLines.at(k); //line without comments
      QString ind = separatedLines.at(k); //line with comments
      
      //indent the current line
      for(int i = 0; i < indentLevel; i++)
      {
         ind.prepend("\t");
      }
      
      int relativeLevel = 0;
      bool reset = false;
      bool decrCurrentLine = false;
         
      foreach(indentKeyword kw, *iw)
      {
         QRegExp expression(kw.pattern);
         int index = expression.indexIn(line); //and NOT ind, as we don't count ANY inserted indentation

         while (index >= 0) {
            int length = expression.matchedLength();
            switch(kw.rule)
            {
               case Increment:
                  relativeLevel += 1;
                  break;
               case IDLine:
                  relativeLevel += 1;
                  if(index == 0) {
                     relativeLevel -= 1;
                     decrCurrentLine = true;
                  }
                  break;
               case Decrement:
                  relativeLevel -= 1;
                  if(index == 0)
                     decrCurrentLine = true;
                  break;
               case Reset:
                  relativeLevel = 0;
                  reset = true;
                  if(index == 0) {
                     while(ind.at(0) == '\t')
                        ind = ind.mid(1);
                  }
                  break;
                  
               default:
                  break;
            }
            index = expression.indexIn(line, index + length);
         }

      }

      if(relativeLevel > 1)
         relativeLevel = 1;
      if(relativeLevel < -1)
         relativeLevel = -1;
      if(reset)
         indentLevel = 0;
      else
         indentLevel += relativeLevel;
      if(decrCurrentLine)
         ind = ind.mid(1);
      
      result << ind;
   }
   return result.join("\n");   
}

QString removeIndent(QString code)
{
   QStringList separatedLines = code.split('\n', QString::KeepEmptyParts);
   QStringList result;
   foreach(QString line, separatedLines)
   {
      QString unind = line;
      while(unind.at(0) == '\t' || unind.at(0) == ' ') {
         unind = unind.mid(1);
      }
      result << unind;
   }
   return result.join("\n");
}

void fillIndentWords(QVector<indentKeyword> *iw)
{
   QStringList triggers;
   QList<indentRule> rules;
   indentKeyword kwd;
   triggers << "\\blet\\b(.*)=((^((?!and).)*$)(\\band\\b(.*)=))*" 
      << "\\btype\\b(.*)=" 
      << "\\bin\\b" 
      << "\\bwith\\b" 
      << "\\(" 
      << "\\)" 
      << "\\{"
      << "\\}"
      << "\\bbegin\\b" 
      << "\\bend\\b" 
      << "\\[(\\|?)" 
      << "(\\|?)\\]" 
      << "\\bfor\\b(.*)\\bdo\\b" 
      << "\\bwhile\\b(.*)\\bdo\\b" 
      << "\\bdone\\b" 
      << "\\btry\\b" 
      << "\\bmatch\\b"
      << "\\bif\\b"
      << "\\bthen\\b"
      << "\\band\\b(.*)="
      << "\\band\\b(.*)=(.*)\\bin\\b"
      << "\\bwhere\\b"
      << ";;";
      
   rules << Increment //let = (and ... =)
      << Increment //type = 
      << Decrement //in
      << Decrement //with : I don't put it since I can't determine where the matching stops. The same applies for the if/then/else construction (that's why there's a decrement for "then").
      << Increment //(
      << Decrement //)
      << Increment //{
      << Decrement //}
      << Increment //begin
      << Decrement //end
      << Increment //[
      << Decrement //]
      << Increment //for do
      << Increment //while do
      << Decrement //done
      << Increment //try
      << Increment //match
      << Increment //if
      << Decrement //then
      << IDLine //and
      << Decrement //and in (to compensate let a = ... and b = .... in that counts twice)
      << IDLine
      << Reset; //;;
   while(!rules.isEmpty())
   {
      kwd.pattern = triggers.takeFirst();
      kwd.rule = rules.takeFirst();
      (*iw) << kwd;
   }
}