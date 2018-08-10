#ifndef SPATIALINDEXALGORITHM_H
#define SPATIALINDEXALGORITHM_H
#include <math.h>
#include "algorithm.h"
#include "boundingbox.h"
#include <unordered_set>

/**
 * @brief The SpatialIndexAlgorithm class executes the spatial indexing method
 * of broad phase collision detection.
 *
 * This class stores the data necessary to track particle movements, and to
 * break the space into and array of non-overlapping subspaces.  At each
 * timestep, it determines for each particle, which subspaces it falls in, then
 * for each subspace, checks each particle in the space against all the others
 * to see if there are collisions.
 */
class SpatialIndexAlgorithm : public Algorithm
{
public:
    /**
     * @brief constructs a 2d SpatialIndexAlgorithm.
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
    SpatialIndexAlgorithm(double xIn, double yIn, int numParticles);
    ~SpatialIndexAlgorithm();

private:
    void calculateNextTimestep();
    int hashFunc(int x, int y, int z);
    int hashFunc(int x, int y);
    double cellSize;
    std::vector<BoundingBox*> boundingBoxes;
};

#endif // SPATIALINDEXALGORITHM_H
