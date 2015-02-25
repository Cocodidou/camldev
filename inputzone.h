// inputzone.cpp - Rich text handler additions for handling keycodes
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

#ifndef INPUTZONE_H
#define INPUTZONE_H

#include <QTextEdit>
#include <QKeyEvent>

class InputZone : public QTextEdit
{
    Q_OBJECT
public:
    explicit InputZone();
    void keyPressEvent(QKeyEvent *event);
    void setHandleEnter(bool ent) { handleEnter = ent; }
    bool getHandleEnter() { return handleEnter; }
private:
   bool handleEnter;
signals:
    void returnPressed();
    void unindentKeyStrokePressed();
public slots:
    
};

#endif // INPUTZONE_H
