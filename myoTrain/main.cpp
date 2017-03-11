 #include "myotrain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myoTrain w;
    w.show();

    return a.exec();
}
