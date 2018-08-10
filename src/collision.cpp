#include "collision.h"

Collision::Collision(Particle* partA, Particle* partB, int timestepIn, double overlapIn)
{
    a = partA;
    b = partB;
    timestep = timestepIn;
    overlapTime = overlapIn;
}

Particle* Collision::getColliderA()
{
    return a;
}

Particle* Collision::getColliderB()
{
    return b;
}

int Collision::getCollisionTimestep()
{
    return timestep;
}

double Collision::getOverlapTime()
{
    return overlapTime;
}

bool Collision::operator <(Collision* other)
{
    return other->getOverlapTime() < overlapTime;
}
