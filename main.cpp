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

#include <QApplication>
#include "camldevwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CamlDevWindow w(a.applicationDirPath());
    if(argc > 1) { //open only the first file...
       QStringList arguments = a.arguments();
       QString fn = arguments[1]; //exists
       w.openFile(fn);
    }
    w.show();

    return a.exec();
}
