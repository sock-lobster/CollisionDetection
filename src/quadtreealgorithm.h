#ifndef QUADTREEALGORITHM_H
#define QUADTREEALGORITHM_H
#include <math.h>
#include "algorithm.h"
#include "quadtree.h"


/**
  * @brief An Algorithm that implements Quadtree%s.
  *
  * An Algorithm that implements Quadtree%s as a way to limit the number of Collision checks.
  */
class QuadtreeAlgorithm : public Algorithm
{
public:
    /**
     * @brief Constructs a QuadtreeAlgorithm object.
     *
     * Constructs a QuadtreeAlgorithm object based on the given parameters.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param numParticlesIn the number of Particle%s.
     */
    QuadtreeAlgorithm(double xIn, double yIn, int numParticlesIn);
    ~QuadtreeAlgorithm();

private:
    Quadtree* quadtree;
    void calculateNextTimestep();
    int hash(int a, int b);
};

#endif // QUADTREEALGORITHM_H
