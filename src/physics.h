#ifndef PHYSICS_H
#define PHYSICS_H
#include <math.h>
#include "particle.h"
#include "constants.h"
#include <boost/random.hpp>
#define _USE_MATH_DEFINES

/**
 * @brief The Physics class
 *
 * The Physics class; parent of Physics2D, Physics3D, and OrbitalPhysics.
 */
class Physics
{
public:
    /**
     * @brief Constructs a Physics object.
     *
     * Constructs a Physics object by setting Orbital to false and seeding random number generators.
     */
    Physics();
    ~Physics();

    /**
     * @brief Returns the centerOfGravity.
     *
     * Returns a pointer to the Particle representing the center of gravity.
     *
     * @return Pointer to the Particle representing the center of gravity.
     */
    Particle* getCenterOfGravity();

    /**
     * @brief Boolean for if the system is orbital.
     *
     * Returns true if the system has orbiting Particle%s; false if not.
     *
     * @return True if system is orbital; false if not.
     */
    bool isOrbital();

    /**
     * @brief Updates the Position and velocity of a Particle.
     *
     * Updates the Position and velocity of a given Particle at a given timestep. Each Algorithm implements it's own version.
     * @param p pointer to the Particle to be updated.
     * @param timestep timestep at which to update the Particle.
     */
    virtual void updatePositionAndVelocity(Particle* p, int timestep) = 0;

    /**
     * @brief Determines whether or not two Particle%s overlap.
     *
     * Returns true if the two Particle%s overlap, otherise false.
     * @param a Pointer to the first Particle.
     * @param b pointer to the second Particle.
     * @return True if Particle%s are ovelapping; false if not.
     */
    bool particlesOverlap(Particle* a, Particle* b);

    /**
     * @brief Creates a Collision between the given Particle%s.
     * @param a a pointer to the first Particle of the Collision.
     * @param b a pointer to the second Particle of the Collision.
     * @param timestep the timestep where the Collision happens.
     * @return A pointer to the Collision object.
     */
    Collision* generateCollision(Particle* a, Particle* b, int timestep);

    /**
     * @brief Resolves the given Collision.
     *
     * Resolves the given Collision by adjusting the Particle%s' Position%s and
     * velocities appropriately.
     * @param collision the Collision to resolve.
     */
    void resolveCollision(Collision *collision);

    /**
     * @brief Returns the relative velocity between two Particle%s.
     *
     * Returns a std::valarray<double> of the relative velocity between two Particle%s given a pointer to each Particle.
     *
     * @param a pointer to first Particle.
     * @param b pointer to second Particle.
     * @return Realtive velocity between two Particle%s.
     */
    std::valarray<double> getRelativeVelocity(Particle* a, Particle* b);

    /**
     * @brief Returns the magnitude of a vector.
     *
     * Returns a double representing the magnitude of a given std::valarray<double>.
     *
     * @param vel velocity vector to get the magnitude of.
     * @return Magnitude of the given vector.
     */
    double getMagnitude(std::valarray<double> vel);

    /**
     * @brief Calculates the kinetic energy of a given Particle.
     *
     * Returns a double representing the kinetic energy given a pointer to a Particle.
     *
     * @param p pointer to a Particle.
     * @return Kinetic energy of a Particle.
     */
    double calculateKE(Particle* p);

    /**
     * @brief Calculates the potential energy of a given Particle.
     *
     * Returns a double representing the potential energy given a pointer to a Particle.
     *
     * @param p pointer to a Particle.
     * @return Potential energy of a Particle.
     */
    virtual double calculatePE(Particle* p);

    /**
     * @brief Sets the Particle%s acceleration based on gravitational forces.
     * @param p pointer to a Particle.
     */
    virtual void initializeAcceleration(Particle* p){};

    /**
     * @brief Generates a random double within a given range.
     *
     * Returns a randomly generated double in a normal distribution between a given min and max.
     *
     * @param min minimum of the range to generate numbers within.
     * @param max maximum of the range to generate numbers within.
     * @return Random number.
     */
    double randDouble(double min, double max);

    /**
     * @brief Generates a random double within a given range.
     *
     * Returns a randomly generated double in a gamma distribution between a given min and max.
     *
     * @param min minimum of the range to generate numbers within.
     * @param max maximum of the range to generate numbers within.
     * @return Random number.
     */
    double randGamma(double mean, double var);

    /**
     * @brief Generates a 0 or 1.
     *
     * Returns 0 or 1 randomly generated in a normal distribution.
     *
     * @return Random bit.
     */
    int randBit();

protected:
    bool orbital;
    Particle* centerOfGravity;

    boost::mt19937 rng;

};

#endif // PHYSICS_H
