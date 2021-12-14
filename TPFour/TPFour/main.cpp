#include "TPFour.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TPFour w;
    w.show();
    return a.exec();
}
