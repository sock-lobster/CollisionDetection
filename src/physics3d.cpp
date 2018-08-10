#include "physics3d.h"

Physics3D::Physics3D(double xIn, double yIn, double zIn):
    Physics()
{
    xWinSize = xIn;
    yWinSize = yIn;
    zWinSize = zIn;
}

Physics3D::~Physics3D(){}

void Physics3D::updatePositionAndVelocity(Particle* p, int timestep)
{
    double newXpos = p->getXpos() + p->getXvel();
    double newYpos = p->getYpos() + p->getYvel();
    double newZPos = p->getZpos() + p->getZvel();
    double newXvel = p->getXvel();
    double newYvel = p->getYvel();
    double newZvel = p->getZvel();

    fixWallCollision(p, newXpos, newYpos, newZPos, newXvel, newYvel, newZvel, timestep);
}

void Physics3D::fixWallCollision(Particle* p, double newXpos, double newYpos, double newZpos, double newXvel, double newYvel, double newZvel, int timestep) {
    double radius = p->getRadius();
    if (newXpos + radius >= xWinSize)
    {
        newXpos -= 2*((newXpos + radius) - xWinSize);
        newXvel = -newXvel;
    }

    else if (newXpos - radius <= 0)
    {
        newXpos += 2 * (radius - newXpos);
        newXvel = -newXvel;
    }

    if (newYpos + radius >= yWinSize)
    {
        newYpos -= 2*((newYpos + radius) - yWinSize);
        newYvel = -newYvel;
    }

    else if (newYpos - radius <= 0)
    {
        newYpos += 2 * (radius - newYpos);
        newYvel = -newYvel;
    }

    if (newZpos + radius >= zWinSize)
    {
        newZpos -= 2*((newZpos + radius) - zWinSize);
        newZvel = -newZvel;
    }

    else if (newZpos - radius <= 0)
    {
        newZpos += 2 * (radius - newZpos);
        newZvel = -newZvel;
    }

    p->updateVel(newXvel, newYvel, newZvel);
    Position* newPos = new Position(newXpos, newYpos, newZpos, timestep);
    p->updateCurPos(newPos);
}
