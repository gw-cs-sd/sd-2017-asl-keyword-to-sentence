#include "displaywords.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    displayWords w;
    w.show();
    return a.exec();
}
