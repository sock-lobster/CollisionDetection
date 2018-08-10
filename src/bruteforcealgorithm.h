#ifndef BRUTEFORCEALGORITHM_H
#define BRUTEFORCEALGORITHM_H
#include <math.h>
#include "algorithm.h"
#define _USE_MATH_DEFINES

/**
 * @brief The BruteForceAlgorithm class executes the simple O(n^2) method.
 *
 * This class stores the data necessary to track Particle movements. At each
 * timestep, it compares the Position of each Particle with that of each other
 * Particle to see if they are farther appart than the sum of ther radii. If
 * not, then a Collision has happened, and it gets resolved.
 */
class BruteForceAlgorithm : public Algorithm
{
public:
    /**
     * @brief Constructs a 2D BruteForceAlgorithm.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number
     * of Particle%s in 2 dimensional space.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    BruteForceAlgorithm(double xIn, double yIn, int numParticles);

    /**
     * @brief Constructs a 3D BruteForceAlgorithm.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number
     * of Particle%s in 3 dimensional space.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    BruteForceAlgorithm(double xIn, double yIn, double zIn, int numParticles);

    /**
     * @brief Constructs a 3D BruteForceAlgorithm with orbital Physics.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number
     * of Particle%s in 3 dimensional space with circular orbits.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param numParticles the number of Particle%s to spawn and track.
     * @param centerMass the mass of the Particle responsible for gravity.
     */
    BruteForceAlgorithm(double xIn, double yIn, double zIn, int numParticles, double centerMass);

    ~BruteForceAlgorithm();

private:
    void calculateNextTimestep();
};

#endif // BRUTEFORCEALGORITHM_H
