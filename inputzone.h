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
signals:
    void controlEnterPressed();
    void controlSPressed();
    void controlOPressed();
    void controlPPressed();
public slots:
    
};

#endif // INPUTZONE_H
