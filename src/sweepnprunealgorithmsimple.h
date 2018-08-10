#ifndef SWEEPNPRUNEALGORITHM_H
#define SWEEPNPRUNEALGORITHM_H
#include <math.h>
#include "algorithm.h"
#include "boundingbox.h"

/**
 * @brief The SweepNPruneAlgorithmSimple class executes the sweep and prune
 * method of Collision detection.
 *
 * This class stores the data necessary to track Particle movements. At each
 * timestep, it sorts the list of Particle%s based on their minimum values in a
 * particular dimension. Based on this sort it compares the Position of only
 * Particle%s that overlap each other in this dimension to see if they are
 * farther appart than the sum of ther radii. If not, then a Collision has
 * happened, and it gets resolved. Insertion sort is close to O(n) time on
 * nearly sorted lists, and the list will always be nearly sorted because the
 * Particle%s move relatively slowly.
 */
class SweepNPruneAlgorithmSimple : public Algorithm
{
public:
    /**
     * @brief Constructs a 2D SweepNPruneAlgorithmSimple.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number
     * of Particle%s in 2-space. Then, for each Particle, it will create a
     * BoundingBox which will be maintained in a sorted list of BoundingBox%es.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    SweepNPruneAlgorithmSimple(double xIn, double yIn, int numParticles);

    /**
     * @brief Constructs a 3D SweepNPruneAlgorithmSimple.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number
     * of Particle%s in 3-space. Then, for each Particle, it will create a
     * BoundingBox which will be maintained in a sorted list of BoundingBox%es.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    SweepNPruneAlgorithmSimple(double xIn, double yIn, double zIn, int numParticles);

    /**
     * @brief Constructs a 3D SweepNPruneAlgorithmSimple with orbital Physics.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number
     * of Particle%s in 3-space with circular orbits. Then, for each Particle,
     * it will create a BoundingBox which will be maintained in a sorted list
     * of BoundingBox%es.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param massIn the mass of the Particle responsible for gravity.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    SweepNPruneAlgorithmSimple(double xIn, double yIn, double zIn, double massIn, int numParticles);

    ~SweepNPruneAlgorithmSimple();

private:
    void calculateNextTimestep();

    std::vector<BoundingBox*> boundingBoxes;
    void insertionSort(std::vector<BoundingBox*> &boxes, int dimension);
};

#endif // SWEEPNPRUNEALGORITHM_H

