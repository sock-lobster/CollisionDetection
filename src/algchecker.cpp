#include <time.h>
#include "algchecker.h"
#include "algorithm.h"
#include "bruteforcealgorithm.h"
#include "spatialhashalgorithm.h"
#include "sweepnprunealgorithmsimple.h"
#include "sweepnprunealgorithmmulti.h"
#include "quadtreealgorithm.h"
#include "spatialhashalgorithm3d.h"
#include "spatialindexalgorithm.h"
#include "spatialindexalgorithm3d.h"
#include "octreealgorithm.h"


AlgChecker::AlgChecker(int dimensions, int algType, double winSize, int numPartsIn, int finalTime, int seedIn)
{
    // Make sure dimensions and algType are valid values to make sure we are actually testing what
    // we think we are.
    if (dimensions < 2 || dimensions > 3){
        std::cout<<"Invalid input: dimensions must equal 2 for 2D or 3 for 3D"<<std::endl;
        exit(0);
    }
    if (algType < 0 || algType > 5){
        std::cout<<"Invalid input: algType must equal {0,1,2,3,4,5}"<<std::endl;
        exit(0);
    }

    // Set up some of the constants
    numParts = numPartsIn;
    finalTimestep = finalTime;

    X_DIMENSION_SIZE = winSize;
    Y_DIMENSION_SIZE = winSize;
    Z_DIMENSION_SIZE = winSize;
    SEED = seedIn;
    MAX_QUEUE_SIZE = (finalTimestep+2) * numParts;
    TIMESTEP_LIMIT = finalTimestep;
    MIN_PARTICLE_RADIUS = 5;
    MAX_PARTICLE_RADIUS = 5;
    LOOKAHEAD_TIME = 0;
    LOOKAHEAD_TIMESTEPS = 0;

    // 2D algorithms
    if (dimensions == 2){
        switch (algType) {
        case 0:
            algorithm = new BruteForceAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, numParts);
            algID = 0;
            break;
        case 1:
            algorithm = new QuadtreeAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, numParts);
            algID = 1;
            break;
        case 2:
            algorithm = new SpatialHashAlgorithm(X_DIMENSION_SIZE,Y_DIMENSION_SIZE, numParts);
            algID = 2;
            break;
        case 3:
            algorithm = new SpatialIndexAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, numParts);
            algID = 3;
            break;
        case 4:
            algorithm = new SweepNPruneAlgorithmSimple(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, numParts);
            algID = 4;
            break;
        case 5:
            algorithm = new SweepNPruneAlgorithmMulti(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, numParts);
            algID = 5;
        }
    }

    // 3D algorithms
    else if (dimensions == 3){
        switch(algType){
            case 0:
                algorithm = new BruteForceAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, numParts);
                algID = 0;
                break;
            case 1:
                algorithm = new OctreeAlgorithm(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, numParts);
                algID = 1;
                break;
            case 2:
                algorithm = new SpatialHashAlgorithm3D(X_DIMENSION_SIZE,Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, numParts);
                algID = 2;
                break;
            case 3:
                algorithm = new SpatialIndexAlgorithm3D(X_DIMENSION_SIZE,Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, numParts);
                algID = 3;
                break;
            case 4:
                algorithm = new SweepNPruneAlgorithmSimple(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, numParts);
                algID = 4;
                break;
            case 5:
                algorithm = new SweepNPruneAlgorithmMulti(X_DIMENSION_SIZE, Y_DIMENSION_SIZE, Z_DIMENSION_SIZE, numParts);
                algID = 5;
        }
    }
}

AlgChecker::~AlgChecker()
{
    delete algorithm;
}

void AlgChecker::run()
{
    clock_t time;
    time = clock();

    algorithm->run();

    time = clock() - time;

//    Every run, outputs "n, t, ch, cl" on a new line where n=number of particles, t=time it took, and c=number of collisions.
    std::cout<<algorithm->getNumParts()<<", "<<(float)time/CLOCKS_PER_SEC<<", "<<algorithm->getChecks()<<", "<<algorithm->getCollisions()->size()<<std::endl;
}
