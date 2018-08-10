#ifndef SPATIALINDEXALGORITHM3D_H
#define SPATIALINDEXALGORITHM3D_H
#include <math.h>
#include "algorithm.h"
#include "boundingbox.h"
#include <unordered_set>

/**
 * @brief The SpatialIndexAlgorithm3D class executes the spatial indexing method
 * of broad phase collision detection.
 *
 * This class stores the data necessary to track particle movements, and to
 * break the space into and array of non-overlapping subspaces.  At each
 * timestep, it determines for each particle, which subspaces it falls in, then
 * for each subspace, checks each particle in the space against all the others
 * to see if there are collisions.
 */
class SpatialIndexAlgorithm3D : public Algorithm
{
public:
    /**
     * @brief constructs a SpatialIndexAlgorithm3D.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number of
     * particles in 3-space.  Then, determines the appropriate size of a cell
     * or subspace given the maximum Particle size, and gives each Particle a
     * BoundingBox.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    SpatialIndexAlgorithm3D(double xIn, double yIn, double zIn, int numParticles);

    /**
     * @brief constructs a SpatialIndexAlgorithm3D with orbital Physics.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number of
     * particles in 3-space.  Then, determines the appropriate size of a cell
     * or subspace given the maximum Particle size, and gives each Particle a
     * BoundingBox.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param numParticles the number of Particle%s to spawn and track.
     * @param centerMass the mass of the Particle responsible for gravity.
     */
    SpatialIndexAlgorithm3D(double xIn, double yIn, double zIn, int numParticles, double centerMass);
    ~SpatialIndexAlgorithm3D();

private:
    void calculateNextTimestep();
    int hashFunc(int x, int y, int z);
    int hashFunc(int x, int y);
    double cellSize;
    std::vector<BoundingBox*> boundingBoxes;
};

#endif // SPATIALINDEXALGORITHM3D_H
