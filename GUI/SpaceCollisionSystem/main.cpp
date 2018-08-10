#include "collisionsystem.h"
#include "algchecker.h"
#include <QApplication>
#include "bruteforcealgorithm.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CollisionSystem w;
    w.show();
    return a.exec();
}
