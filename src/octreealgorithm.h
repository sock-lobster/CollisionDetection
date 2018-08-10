#ifndef OCTREEALGORITHM_H
#define OCTREEALGORITHM_H
#include "algorithm.h"
#include "octree.h"

/**
  * @brief An Algorithm that implements Octree%s.
  *
  * An Algorithm that implements Octree%s as a way to limit the number of Collision checks.
  */
class OctreeAlgorithm : public Algorithm
{
public:
    /**
     * @brief Constructs a OctreeAlgorithm object.
     *
     * Constructs a OctreeAlgorithm object based on the given parameters.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param numParticlesIn the number of Particle%s.
     */
    OctreeAlgorithm(double xIn, double yIn, double zIn, int numParticlesIn);

    /**
     * @brief Constructs a OctreeAlgorithm object.
     *
     * Constructs a OctreeAlgorithm object based on the given parameters.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param centerMassIn the mass of the Particle that represents the center of gravity.
     * @param numParticlesIn the number of Particle%s.
     */
    OctreeAlgorithm(double xIn, double yIn, double zIn, double centerMassIn, int numParticlesIn);
    ~OctreeAlgorithm();

private:
    Octree* octree;
    void calculateNextTimestep();
    int hash(int a, int b);
};

#endif // OCTREEALGORITHM_H
