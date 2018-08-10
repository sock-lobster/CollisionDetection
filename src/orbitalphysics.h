#ifndef ORBITALPHYSICS_H
#define ORBITALPHYSICS_H
#include "physics.h"

/**
 * @brief The OrbitalPhysics class is a Physics for three dimensional motion
 * with gravity.
 *
 * This physics is the same as the other Physics, but here the Particle%s are
 * attracted by the center of gravity and accelerate towards it with each
 * timestep.
 */
class OrbitalPhysics : public Physics
{
public:
    /**
     * @brief Constructs a Physics3D object with gravity.
     *
     * This calls Physics::Physics(), then initializes the size of the space,
     * then creates the Particle responsible for gravitation.
     *
     * @param xWinSize the size of the x dimension of this space.
     * @param yWinSize the size of the y dimension of this space.
     * @param zWinSize the size of the z dimension of this space.
     * @param massIn the mass of the Particle responsible for gravity.
     */
    OrbitalPhysics(double xWinSize, double yWinSize, double zWinSize, double massIn);
    ~OrbitalPhysics();

    /**
     * @brief Calculates the potential energy of the given Particle.
     *
     * Based on the gravitational field, and the Particle's characteristics,
     * this determines the amount of potential energy. Note, potential energy
     * is always a negative value, with a Particle infinitely far from the
     * center of gravity having 0 potential energy.
     *
     * @param p Particle we are calculating the potential energy for.
     * @return The gravitational potential energy of a particle.
     */
    double calculatePE(Particle* p);

private:
    double xWinSize;
    double yWinSize;
    double zWinSize;
    double centerMass;

    void updatePositionAndVelocity(Particle* p, int timestep);
    std::valarray<double> calculateGravityAcceleration(Particle* p);
    void initializeAcceleration(Particle* p);
};

#endif // ORBITALPHYSICS_H
