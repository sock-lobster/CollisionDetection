#include "orbitalphysics.h"


OrbitalPhysics::OrbitalPhysics(double xIn, double yIn, double zIn, double massIn):
    Physics()
{
    xWinSize = xIn;
    yWinSize = yIn;
    zWinSize = zIn;
    centerMass = massIn;
    centerOfGravity = new Particle(-1, new Position(xIn/2, yIn/2, zIn/2, 0), std::valarray<double> (3), 10.0, massIn);

    orbital = true;
}

OrbitalPhysics::~OrbitalPhysics(){}

void OrbitalPhysics::initializeAcceleration(Particle* p)
{
    p->updateAcceleration(calculateGravityAcceleration(p));
}

void OrbitalPhysics::updatePositionAndVelocity(Particle* p, int timestep)
{
    std::valarray<double> acceleration = calculateGravityAcceleration(p);

    double newXpos = p->getXpos() + p->getXvel() + 0.5 * p->getAcceleration()[X_DIMENSION];
    double newYpos = p->getYpos() + p->getYvel() + 0.5 * p->getAcceleration()[Y_DIMENSION];
    double newZpos = p->getZpos() + p->getZvel() + 0.5 * p->getAcceleration()[Z_DIMENSION];

    double newXvel = p->getXvel() + 0.5 * (acceleration[X_DIMENSION] + p->getAcceleration()[X_DIMENSION]);
    double newYvel = p->getYvel() + 0.5 * (acceleration[Y_DIMENSION] + p->getAcceleration()[Y_DIMENSION]);
    double newZvel = p->getZvel() + 0.5 * (acceleration[Z_DIMENSION] + p->getAcceleration()[Z_DIMENSION]);

    p->updateVel(newXvel, newYvel, newZvel);
    Position* newPos = new Position(newXpos, newYpos, newZpos, timestep);
    p->updateCurPos(newPos);
    p->updateAcceleration(acceleration);
}

std::valarray<double> OrbitalPhysics::calculateGravityAcceleration(Particle* p)
{
    std::valarray<double> direction = p->getCurPos()->getPos() - centerOfGravity->getCurPos()->getPos();
    std::valarray<double> acceleration (0.0, 3);
    double radius = getMagnitude(direction);
    if (radius == 0)
    {
        return acceleration;
    }
    acceleration = -(direction / radius) * (GRAVITY * centerMass) / pow(radius, 2);
    return acceleration;
}

double OrbitalPhysics::calculatePE(Particle* p)
{
    std::valarray<double> direction = p->getCurPos()->getPos() - centerOfGravity->getCurPos()->getPos();
    return -GRAVITY * centerMass * p->getMass() / getMagnitude(direction);
}

