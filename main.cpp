#include <QApplication>
#include "camldevwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CamlDevWindow w(a.applicationDirPath());
    if(argc > 1) { //open only the first file...
       QString fn(argv[1]);
       w.openFile(fn);
    }
    w.show();

    return a.exec();
}
