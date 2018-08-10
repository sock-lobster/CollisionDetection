#ifndef ALGCHECKER_H
#define ALGCHECKER_H
#include "algorithm.h"
#include "constants.h"

/**
 * @brief The AlgChecker class is used to perform tests on a given Algorithm
 * under the specific environment created by the given values.
 *
 * Test results are printed to stdout.
 */
class AlgChecker
{
public:
    /**
     * @brief Constructs the Algorithm and environment specified by input.
     * @param dimensions 2 for 2D, 3 for 3D, 4 for Orbital
     * @param algType 0 for BruteForceAlgorithm,
     *                1 for QuadtreeAlgorithm / OctreeAlgorithm,
     *                2 for SpatialHashAlgorithm / SpatialHashAlgorithm3D,
     *                3 for SpatialIndexAlgorithm / SpatialIndexAlgorithm3D,
     *                4 for SweepNPruneAlgorithmSimple,
     *                5 for SweepNPruneAlgorithmMulti
     * @param winSize the length of each side of the bounding space
     * @param numPartsIn the number of Particle%s to exist in the environment
     * @param finalTime the number of timesteps to run the Algorithm for
     * @param seedIn the seed for the random generator
     */
    AlgChecker(int dimensions, int algType, double winSize, int numPartsIn, int finalTime, int seedIn);

    ~AlgChecker();

    /**
     * @brief Calculates the time it takes the already constructed Algorithm to
     * complete the given number of timesteps under the constructed environment.
     *
     * Outputs to stdout a new line "n,t,c" where
     *                n = the number of Particle%s in the environment,
     *                t = the time it took to complete, and
     *                c = the number of Collision%s the Algorithm detected
     */
    void run();

private:
    Algorithm* algorithm;
    int finalTimestep;
    int numParts;
    int algID;

};

#endif // ALGCHECKER_H
