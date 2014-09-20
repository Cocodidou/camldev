#include "inputzone.h"

InputZone::InputZone() :
    QTextEdit()
{

}

void InputZone::keyPressEvent(QKeyEvent *event)
{
    if(event->key()== Qt::Key_Return && event->modifiers() == Qt::ControlModifier)
    {
        emit controlEnterPressed();
    }
    else if(event->key()== Qt::Key_S && event->modifiers() == Qt::ControlModifier)
    {
        emit controlSPressed();
    }
    else if(event->key()== Qt::Key_O && event->modifiers() == Qt::ControlModifier)
    {
        emit controlOPressed();
    }
    else if(event->key()== Qt::Key_P && event->modifiers() == Qt::ControlModifier)
    {
        emit controlPPressed();
    }
    else if(event->key()== Qt::Key_Plus && event->modifiers() & Qt::ControlModifier)
    {
        emit controlPlusPressed();
    }
    else if(event->key()== Qt::Key_Minus && event->modifiers() & Qt::ControlModifier )
    {
        emit controlMinusPressed();
    }
    else
    {
        QTextEdit::keyPressEvent( event );
    }
}
