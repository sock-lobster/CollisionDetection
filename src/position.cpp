#include "position.h"

Position::~Position() {}

Position::Position(double xIn, double yIn, double zIn, int timestepIn)
{
    pos.resize(3, 0);
    pos[0] = xIn;
    pos[1] = yIn;
    pos[2] = zIn;
    timestep = timestepIn;
}

Position::Position(Position* position) {
    pos.resize(3, 0);
    pos[0] = position->getX();
    pos[1] = position->getY();
    pos[2] = position->getZ();
    timestep = position->timestep;
}

double Position::getX()
{
    return pos[0];
}

double Position::getY()
{
    return pos[1];
}
double Position::getZ()
{
    return pos[2];
}

std::valarray<double> Position::getPos()
{
    return pos;
}

int Position::getTimestep()
{
    return timestep;
}
