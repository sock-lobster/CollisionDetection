#include "sweepnprunealgorithmmulti.h"

SweepNPruneAlgorithmMulti::SweepNPruneAlgorithmMulti(double xIn, double yIn, int numParticles) :
    Algorithm(xIn, yIn)
{
    maxDimension = Y_DIMENSION;
    createParticles2D(numParticles);

    // unique to sweepN'prune:
    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }

    for (BoundingBox* b : boundingBoxes)
    {
        boundariesX.push_back(new Boundary(X_DIMENSION, b->getMin(X_DIMENSION), false, b));
        boundariesX.push_back(new Boundary(X_DIMENSION, b->getMax(X_DIMENSION), true, b));

        boundariesY.push_back(new Boundary(Y_DIMENSION, b->getMin(Y_DIMENSION), false, b));
        boundariesY.push_back(new Boundary(Y_DIMENSION, b->getMax(Y_DIMENSION), true, b));
    }
    buildMap();
}

SweepNPruneAlgorithmMulti::SweepNPruneAlgorithmMulti(double xIn, double yIn, double zIn, int numParticles) :
    Algorithm(xIn, yIn, zIn)
{
    maxDimension = Z_DIMENSION;
    createParticles3D(numParticles);

    // unique to sweepN'prune:
    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }

    for (BoundingBox* b : boundingBoxes)
    {
        boundariesX.push_back(new Boundary(X_DIMENSION, b->getMin(X_DIMENSION), false, b));
        boundariesX.push_back(new Boundary(X_DIMENSION, b->getMax(X_DIMENSION), true, b));

        boundariesY.push_back(new Boundary(Y_DIMENSION, b->getMin(Y_DIMENSION), false, b));
        boundariesY.push_back(new Boundary(Y_DIMENSION, b->getMax(Y_DIMENSION), true, b));

        boundariesZ.push_back(new Boundary(Z_DIMENSION, b->getMin(Z_DIMENSION), false, b));
        boundariesZ.push_back(new Boundary(Z_DIMENSION, b->getMax(Z_DIMENSION), true, b));
    }
    buildMap();
}

SweepNPruneAlgorithmMulti::SweepNPruneAlgorithmMulti(double xIn, double yIn, double zIn, double centerMassIn, int numParticles) :
    Algorithm(xIn, yIn, zIn, centerMassIn)
{
    maxDimension = Z_DIMENSION;
    createParticlesOrbit(numParticles);

    // unique to sweepN'prune:
    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }

    for (BoundingBox* b : boundingBoxes)
    {
        boundariesX.push_back(new Boundary(X_DIMENSION, b->getMin(X_DIMENSION), false, b));
        boundariesX.push_back(new Boundary(X_DIMENSION, b->getMax(X_DIMENSION), true, b));

        boundariesY.push_back(new Boundary(Y_DIMENSION, b->getMin(Y_DIMENSION), false, b));
        boundariesY.push_back(new Boundary(Y_DIMENSION, b->getMax(Y_DIMENSION), true, b));

        boundariesZ.push_back(new Boundary(Z_DIMENSION, b->getMin(Z_DIMENSION), false, b));
        boundariesZ.push_back(new Boundary(Z_DIMENSION, b->getMax(Z_DIMENSION), true, b));
    }
    buildMap();
}


SweepNPruneAlgorithmMulti::~SweepNPruneAlgorithmMulti()
{
    while (!boundingBoxes.empty())
    {
        BoundingBox* b = boundingBoxes.back();
        delete b;
        boundingBoxes.pop_back();
    }

    while (!boundariesX.empty())
    {
        Boundary* bx = boundariesX.back();
        delete bx;
        boundariesX.pop_back();
    }

    while (!boundariesY.empty())
    {
        Boundary* by = boundariesY.back();
        delete by;
        boundariesY.pop_back();
    }

    if (maxDimension == Z_DIMENSION)
    {
        while (!boundariesZ.empty())
        {
            Boundary* bz = boundariesZ.back();
            delete bz;
            boundariesZ.pop_back();
        }
    }

    possibleCollisions.clear();
}

void SweepNPruneAlgorithmMulti::buildMap()
{
    insertionSort(boundingBoxes, X_DIMENSION);

    std::vector<std::pair<BoundingBox*, BoundingBox*>*> overlaps;

    std::vector<BoundingBox*> activeList;

    for (BoundingBox* box : boundingBoxes)
    {
        for (int i = 0; i < (int)activeList.size(); i++)
        {
            if (activeList[i]->getMax(X_DIMENSION) < box->getMin(X_DIMENSION))
            {
                activeList.erase(activeList.begin() + i);
                i--;
            }
            else
            {
                overlaps.push_back(new std::pair<BoundingBox*, BoundingBox*>(activeList[i], box));
            }
        }
        activeList.push_back(box);
    }

    for (int i = 0; i < (int)overlaps.size(); i++)
    {
        if (overlaps[i]->first->boxOverlap3D(overlaps[i]->second))
        {
            int key = hash(overlaps[i]->first->getParticle()->getID(), overlaps[i]->second->getParticle()->getID());
            std::pair<int, std::pair<BoundingBox*, BoundingBox*>*> pair(key, overlaps[i]);
            possibleCollisions.insert(pair);
        }
    }
}

void SweepNPruneAlgorithmMulti::calculateNextTimestep()
{
    timestep++;

    for (int i = 0; i < numParts; i++)
    {
        physics->updatePositionAndVelocity(particles[i], timestep);
    }

    for (BoundingBox* box : boundingBoxes)
    {
        box->update();
    }

    for (Boundary* bound : boundariesX)
    {
        bound->update();
    }

    for (Boundary* bound : boundariesY)
    {
        bound->update();
    }

    if (maxDimension == Z_DIMENSION)
    {
        for (Boundary* bound : boundariesZ)
        {
            bound->update();
        }
    }

    insertionSort(boundariesX);
    insertionSort(boundariesY);
    if (maxDimension == Z_DIMENSION)
    {
        insertionSort(boundariesZ);
    }
    std::vector<Collision*> newCollisions;

    int numchecks = possibleCollisions.size();

    if (numchecks > 0)
    {
        for (auto it = possibleCollisions.begin(); it != possibleCollisions.end(); ++it)
        {
            Particle* a = it->second->first->getParticle();
            Particle* b = it->second->second->getParticle();

            checks++;
            if (physics->particlesOverlap(a, b))
            {
                Collision* newCollision = physics->generateCollision(a, b, timestep);
                newCollisions.push_back(newCollision);
            }
        }
    }

    std::sort(newCollisions.begin(), newCollisions.end());
    for (Collision* c : newCollisions) {
        physics->resolveCollision(c);
    }
    enqueuePositions();
    enqueueCollisions(newCollisions);
}

void SweepNPruneAlgorithmMulti::insertionSort(std::vector<Boundary*>& bounds)
{
    int i;
    int j;
    Boundary* temp;

    for (i = 1; i < (int)bounds.size(); i++)
    {
        j = i;
        while (j >=1 && bounds[j]->getValue() < bounds[j-1]->getValue())
        {
            // new overlap potential
            if (bounds[j]->isLower() && bounds[j-1]->isUpper())
            {
                // boundingBox check is very similar in function to particle overlap check
                checks++;

                // they actually overlap bounding boxes
                if ((maxDimension == Z_DIMENSION && bounds[j]->getBox()->boxOverlap3D(bounds[j-1]->getBox())) || (maxDimension == Y_DIMENSION && bounds[j]->getBox()->boxOverlap2D(bounds[j-1]->getBox())))
                {
                    int key = hash(bounds[j]->getBox()->getParticle()->getID(), bounds[j-1]->getBox()->getParticle()->getID());
                    std::pair<BoundingBox*, BoundingBox*>* colliders = new std::pair<BoundingBox*, BoundingBox*> (bounds[j]->getBox(), bounds[j-1]->getBox());
                    possibleCollisions[key] = colliders;
                }
            }
            // formerly overlaping things arent' any more
            else if (bounds[j]->isUpper() && bounds[j-1]->isLower())
            {
                int key = hash(bounds[j]->getBox()->getParticle()->getID(), bounds[j-1]->getBox()->getParticle()->getID());
                if (possibleCollisions.count(key) >= 1)
                {
                    delete possibleCollisions[key];
                    possibleCollisions.erase(key);
                }
                if (possibleCollisions.count(key) > 1)
                {
                    std::cout << possibleCollisions.count(key) << std::endl;
                }
            }
            temp = bounds[j];
            bounds[j] = bounds[j-1];
            bounds[j-1] = temp;
            j--;
        }
    }
}

void SweepNPruneAlgorithmMulti::insertionSort(std::vector<BoundingBox*>& boxes, int dimension)
{
    int i;
    int j;
    BoundingBox* temp;

    for (i = 1; i < (int)boxes.size(); i++)
    {
        j = i;
        while (j >= 1 && boxes[j]->getMin(dimension) < boxes[j-1]->getMin(dimension))
        {
            temp = boxes[j];
            boxes[j] = boxes[j-1];
            boxes[j-1] = temp;
            j--;
        }
    }
}

int SweepNPruneAlgorithmMulti::hash(int a, int b)
{
    if (a < b)
    {
        return a * 1640531513 + b * 2654435789;
    }
    else
    {
        return b * 1640531513 + a * 2654435789;
    }
}
