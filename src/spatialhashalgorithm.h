#ifndef SPATIALHASHALGORITHM_H
#define SPATIALHASHALGORITHM_H
#include <math.h>
#include <unordered_map>
#include "algorithm.h"
#include "boundingbox.h"
#include <unordered_set>

/**
 * @brief The SpatialHashAlgorithm class executes the spatial hashing method
 * of broad phase collision detection.
 *
 * This class stores the data necessary to track particle movements, and to
 * break the space into a structure storing non-overlapping subspaces.  At each
 * timestep, it determines for each particle, which subspaces it falls in, then
 * for each subspace, checks each particle in the space against all the others
 * to see if there are collisions.
 */
class SpatialHashAlgorithm : public Algorithm
{
public:
    /**
     * @brief constructs a SpatialHashAlgorithm.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number of
     * particles in 3-space.  Then, determines the appropriate size of a cell
     * or subspace given the maximum Particle size, and gives each Particle a
     * BoundingBox.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    SpatialHashAlgorithm(double xIn, double yIn, int numParticles);
    ~SpatialHashAlgorithm();

private:
    void calculateNextTimestep();
    double cellSize;
    int hashFunc(int x, int y);
    std::vector<BoundingBox*> boundingBoxes;
};

#endif // SPATIALHASHALGORITHM_H
