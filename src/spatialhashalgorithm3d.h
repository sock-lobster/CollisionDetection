#ifndef SPATIALHASHALGORITHM3D_H
#define SPATIALHASHALGORITHM3D_H
#include <math.h>
#include <unordered_map>
#include "algorithm.h"
#include "boundingbox.h"
#include <unordered_set>

/**
 * @brief The SpatialHashAlgorithm3D class executes the spatial hashing method
 * of broad phase collision detection.
 *
 * This class stores the data necessary to track particle movements, and to
 * break the space into a structure storing non-overlapping subspaces.  At each
 * timestep, it determines for each particle, which subspaces it falls in, then
 * for each subspace, checks each particle in the space against all the others
 * to see if there are collisions.
 */
class SpatialHashAlgorithm3D : public Algorithm
{
public:
    /**
     * @brief constructs a SpatialHashAlgorithm3D.
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
    SpatialHashAlgorithm3D(double xIn, double yIn, double zIn, int numParticles);

    /**
     * @brief constructs a SpatialHaxhAlgorithm3D with orbital Physics.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number of
     * particles in 3-space.  Then, determines the appropriate size of a cell
     * or subspace given the maximum Particle size, and gives each Particle a
     * BoundingBox.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param centerMass the mass of the Particle responsible for gravity.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    SpatialHashAlgorithm3D(double xIn, double yIn, double zIn, int numParticles, double centerMass);

    ~SpatialHashAlgorithm3D();

private:
    void calculateNextTimestep();
    int hashFunc(int x, int y, int z);
    int hashFunc(int x, int y);
    double cellSize;
    std::vector<BoundingBox*> boundingBoxes;

};

#endif // SPATIALHASHALGORITHM3D_H
