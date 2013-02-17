#include <QApplication>
#include "camldevwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CamlDevWindow w;
    w.show();
    return a.exec();
}
