#ifndef SWEEPNPRUNEALGORITHMMULTI_H
#define SWEEPNPRUNEALGORITHMMULTI_H
#include <math.h>
#include <unordered_map>
#include "algorithm.h"
#include "boundingbox.h"
#include "boundary.h"


/**
 * @brief The SweepNPruneAlgorithmMulti class executes the sweep and prune
 * method of Collision detection in multiple dimensions.
 *
 * This class stores the data necessary to track Particle movements. At each
 * timestep, it sorts the lists of Boundary%s, and when it swaps the order of
 * Boundary%s, it may add or remove the associated pair of BouningBox%es to/
 * from the map of possible Collision%s that stores overlaped pairs of
 * BoundingBox%es. This is done in each dimension, then only pairs of
 * BoundingBox%es that overlap in all the dimensions have their Particle%s
 * checked for a Collision. The sorts are fast because the lists stay nearly
 * sorted at each timestep, and this method greatly reduces the number of
 * checks necessary to find all colliding Particle%s.
 */
class SweepNPruneAlgorithmMulti : public Algorithm
{
public:
    /**
     * @brief Constructs a 2D SweepNPruneAlgorithmMulti.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number
     * of Particle%s in 2-space. Then, for each Particle, it will create a
     * BoundingBox which will be maintained in a sorted list of BoundingBox%es.
     * Then with the BoundingBox%es we get Boundary%s (both upper and lower)
     * for each dimension, and for each dimension make a list of Bounary%s.
     * Finally, we initialize a hash of pairs of overlapping BouningBox%es.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    SweepNPruneAlgorithmMulti(double xIn, double yIn, int numParticles);

    /**
     * @brief Constructs a 3D SweepNPruneAlgorithmMulti.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number
     * of Particle%s in 3-space. Then, for each Particle, it will create a
     * BoundingBox which will be maintained in a sorted list of BoundingBox%es.
     * Then with the BoundingBox%es we get Boundary%s (both upper and lower)
     * for each dimension, and for each dimension make a list of Bounary%s.
     * Finally, we initialize a hash of pairs of overlapping BouningBox%es.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    SweepNPruneAlgorithmMulti(double xIn, double yIn, double zIn, int numParticles);

    /**
     * @brief Constructs a 3D SweepNPruneAlgorithmMulti with orbital Physics.
     *
     * This calls Algorithm::Algorithm() then creates the appropriate number
     * of Particle%s in 3-space with circular orbits. Then, for each Particle,
     * it will create a BoundingBox which will be maintained in a sorted list
     * of BoundingBox%es. Then with the BoundingBox%es we get Boundary%s (both
     * upper and lower) for each dimension, and for each dimension make a list
     * of Bounary%s. Finally, we initialize a hash of pairs of overlapping
     * BouningBox%es.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param CenterMassIn the mass of the Particle responsible for gravity.
     * @param numParticles the number of Particle%s to spawn and track.
     */
    SweepNPruneAlgorithmMulti(double xIn, double yIn, double zIn, double centerMassIn, int numParticles);
    ~SweepNPruneAlgorithmMulti();

private:
    void calculateNextTimestep();
    int maxDimension;

    std::vector<BoundingBox*> boundingBoxes;
    void insertionSort(std::vector<BoundingBox*>& boxes, int dimension);
    std::vector<Boundary*> boundariesX;
    std::vector<Boundary*> boundariesY;
    std::vector<Boundary*> boundariesZ;
    void insertionSort(std::vector<Boundary*>& bounds);

    std::unordered_map<int, std::pair<BoundingBox*, BoundingBox*>*> possibleCollisions;
    void buildMap();
    void updateMap(int dimension);
    int hash(int a, int b);
};

#endif // SWEEPNPRUNEALGORITHMMULTI_H
