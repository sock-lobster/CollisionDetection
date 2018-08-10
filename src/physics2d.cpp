#include "physics2d.h"

Physics2D::Physics2D(double xIn, double yIn):
    Physics()
{
    xWinSize = xIn;
    yWinSize = yIn;
}

Physics2D::~Physics2D() {}

void Physics2D::updatePositionAndVelocity(Particle* p, int timestep)
{
    double newXpos = p->getXpos() + p->getXvel();
    double newYpos = p->getYpos() + p->getYvel();
    double newXvel = p->getXvel();
    double newYvel = p->getYvel();

    fixWallCollision(p, newXpos, newYpos, newXvel, newYvel, timestep);
}

void Physics2D::fixWallCollision(Particle* p, double newXpos, double newYpos, double newXvel, double newYvel, int timestep) {
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

    p->updateVel(newXvel, newYvel, 0.0);
    Position* newPos = new Position(newXpos, newYpos, 0.0, timestep);
    p->updateCurPos(newPos);
}
