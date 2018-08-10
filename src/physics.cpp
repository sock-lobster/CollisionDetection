#include "physics.h"
#include <boost/generator_iterator.hpp>
#include <boost/math/distributions/gamma.hpp>

Physics::Physics()
{
    orbital = false;
    rng.seed(SEED);
}

Physics::~Physics(){}

Particle* Physics::getCenterOfGravity()
{
    return centerOfGravity;
}

bool Physics::isOrbital() {
    return orbital;
}

bool Physics::particlesOverlap(Particle* a, Particle* b)
{
    double squareDistance = pow(a->getCurPos()->getPos() - b->getCurPos()->getPos(), 2.0).sum();
    double squareRadiusSum = pow(a->getRadius() + b->getRadius(), 2);
    return (squareRadiusSum > squareDistance);
}

Collision* Physics::generateCollision(Particle* a, Particle* b, int timestep) {
    // use trig to find elapsed time, with which calculate backup distance
    std::valarray<double> relativeVelocity = getRelativeVelocity(a, b);

    double totRadius = a->getRadius() + b->getRadius();
    double angleA = 0.0;
    double angleB = 0.0;
    std::valarray<double> centerpointsVect (3);
    centerpointsVect[0] = b->getXpos() - a->getXpos();
    centerpointsVect[1] = b->getYpos() - a->getYpos();
    centerpointsVect[2] = b->getZpos() - a->getZpos();


    double dotProd = (relativeVelocity * centerpointsVect).sum();
    double relVelMag = getMagnitude(relativeVelocity); // magninude of relative velocity
    double centerpointDist = getMagnitude(centerpointsVect); // Distance between center points
    double angleC = M_PI - (acos(dotProd / (relVelMag * centerpointDist))); // angle of relative velocity to line between center points
    double distanceToTangent = 0.0;

    angleA = (asin(centerpointDist * sin(angleC) / totRadius)); // angle of relative velocity to line between center points when the particles were tangent
    angleB = M_PI - (angleA + angleC); // third angle of triangle, necessary for law of cosines
    distanceToTangent = pow(pow(centerpointDist, 2) + pow(totRadius, 2) - (2 * centerpointDist * totRadius * cos(angleB)), 0.5); // distance of overlap based on relative velocity

    // move a and b back to tangent position
    double timeElapsed = distanceToTangent / relVelMag;  // amount of time since the particles were tangent

    Collision* collision = new Collision(a, b, timestep, timeElapsed);
    return collision;
}

void Physics::resolveCollision(Collision* collision)
{
    Particle* a = collision->getColliderA();
    Particle* b = collision->getColliderB();
    if (!particlesOverlap(a, b)) {
        return;
    }

    int timestep = collision->getCollisionTimestep();
    double timeElapsed = collision->getOverlapTime();

    double totRadius = a->getRadius() + b->getRadius();

    Position* tanAPos = new Position(a->getXpos() - (a->getXvel() * timeElapsed), a->getYpos() - (a->getYvel() * timeElapsed), a->getZpos() - (a->getZvel() * timeElapsed), timestep);
    Position* tanBPos = new Position(b->getXpos() - (b->getXvel() * timeElapsed), b->getYpos() - (b->getYvel() * timeElapsed), b->getZpos() - (b->getZvel() * timeElapsed), timestep);
    a->updateCurPos(tanAPos);
    b->updateCurPos(tanBPos);

    std::valarray<double> collisionNormal(3);
    collisionNormal[0] = a->getXpos() - b->getXpos();
    collisionNormal[1] = a->getYpos() - b->getYpos();
    collisionNormal[2] = a->getZpos() - b->getZpos();

    collisionNormal /= totRadius;

    double aNormalVelocity = (a->getVel() * collisionNormal).sum();
    double bNormalVelocity = (b->getVel() * collisionNormal).sum();

    std::valarray<double> aNormalVector = aNormalVelocity * collisionNormal;
    std::valarray<double> bNormalVector = bNormalVelocity * collisionNormal;

    std::valarray<double>aTangentVector = a->getVel() - aNormalVector;
    std::valarray<double>bTangentVector = b->getVel() - bNormalVector;

    double aNormalVelPost = (aNormalVelocity * (a->getMass() - b->getMass()) + 2 * b->getMass() * bNormalVelocity) / (a->getMass() + b->getMass());
    double bNormalVelPost = (bNormalVelocity * (b->getMass() - a->getMass()) + 2 * a->getMass() * aNormalVelocity) / (a->getMass() + b->getMass());

    std::valarray<double> aFinal = (aNormalVelPost * collisionNormal) + aTangentVector;
    std::valarray<double> bFinal = (bNormalVelPost * collisionNormal) + bTangentVector;

    a->updateVel(aFinal[0], aFinal[1], aFinal[2]);
    b->updateVel(bFinal[0], bFinal[1], bFinal[2]);

    Position* aUpdatedPos = new Position(a->getXpos() + (a->getXvel() * timeElapsed), a->getYpos() + (a->getYvel() * timeElapsed), a->getZpos() + (a->getZvel() * timeElapsed), timestep);
    Position* bUpdatedPos = new Position(b->getXpos() + (b->getXvel() * timeElapsed), b->getYpos() + (b->getYvel() * timeElapsed), b->getZpos() + (b->getZvel() * timeElapsed), timestep);
    a->updateCurPos(aUpdatedPos);
    b->updateCurPos(bUpdatedPos);
}

std::valarray<double> Physics::getRelativeVelocity(Particle* a, Particle* b)
{
    return a->getVel() - b->getVel();
}

double Physics::getMagnitude(std::valarray<double> vel)
{
    return pow(pow(vel, 2.0).sum(), 0.5);
}

double Physics::calculateKE(Particle* p)
{
    return (p->getMass() * pow(getMagnitude(p->getVel()), 2.0)) / 2;
}

double Physics::calculatePE(Particle* p)
{
    return 0.0;
}

double Physics::randDouble(double min, double max)
{
    if (min == max)
    {
        return min;
    }
    else
    {
        boost::uniform_real<> distr(min, max);
        return distr(rng);
    }
}

double Physics::randGamma(double mean, double var)
{
    boost::gamma_distribution<> gd( mean );
    boost::variate_generator<boost::mt19937&,boost::gamma_distribution<> > var_gamma( rng, gd );

    return var*var_gamma();
}

int Physics::randBit() {
    boost::uniform_int<> distr(0, 1);
    return distr(rng);
}
