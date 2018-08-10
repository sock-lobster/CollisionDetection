#include "collisiondisplay.h"

CollisionDisplay::CollisionDisplay()
{
    timer = new QTimer;
    numCollisions = 0;
    displayTimestep = 0;
    qalg = NULL;
}

CollisionDisplay::~CollisionDisplay() {
    delete timer;
}

QTimer* CollisionDisplay::getTimer() {
    return timer;
}

int CollisionDisplay::getDisplayTimestep()
{
    return displayTimestep;
}

int CollisionDisplay::getNumCollisions() {
    return numCollisions;
}

void CollisionDisplay::setQAlgorithm(QAlgorithm *qalgorithm) {
    qalg = qalgorithm;
}
