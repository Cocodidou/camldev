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
    else
    {
        QTextEdit::keyPressEvent( event );
    }
}
