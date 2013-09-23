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
    else
    {
        QTextEdit::keyPressEvent( event );
    }
}
