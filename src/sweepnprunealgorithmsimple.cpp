#include "sweepnprunealgorithmsimple.h"

SweepNPruneAlgorithmSimple::SweepNPruneAlgorithmSimple(double xIn, double yIn, int numParticles) :
    Algorithm(xIn, yIn)
{
    createParticles2D(numParticles);

    // unique to sweepN'prune:
    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }
}

SweepNPruneAlgorithmSimple::SweepNPruneAlgorithmSimple(double xIn, double yIn, double zIn, int numParticles) :
    Algorithm(xIn, yIn, zIn)
{
    createParticles3D(numParticles);

    // unique to sweepN'prune:
    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }
}

SweepNPruneAlgorithmSimple::SweepNPruneAlgorithmSimple(double xIn, double yIn, double zIn, double massIn, int numParticles) :
    Algorithm(xIn, yIn, zIn, massIn)
{
    createParticlesOrbit(numParticles);

    // unique to sweepN'prune:
    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }
}

SweepNPruneAlgorithmSimple::~SweepNPruneAlgorithmSimple()
{
    while (!boundingBoxes.empty())
    {
        BoundingBox* b = boundingBoxes.back();
        delete b;
        boundingBoxes.pop_back();
    }
}

void SweepNPruneAlgorithmSimple::calculateNextTimestep()
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

    insertionSort(boundingBoxes, X_DIMENSION);

    std::vector<std::pair<BoundingBox*, BoundingBox*>> possibleCollisions;

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
                possibleCollisions.push_back(std::pair<BoundingBox*, BoundingBox*>(activeList[i], box));
            }
        }
        activeList.push_back(box);
    }

    std::vector<Collision*> newCollisions;

    for (int i = 0; i < (int)possibleCollisions.size(); i++)
    {
        Particle* a = possibleCollisions[i].first->getParticle();
        Particle* b = possibleCollisions[i].second->getParticle();

        checks++;
        if (physics->particlesOverlap(a, b))
        {
            Collision* newCollision = physics->generateCollision(a, b, timestep);
            newCollisions.push_back(newCollision);
        }
    }
    std::sort(newCollisions.begin(), newCollisions.end());
    for (Collision* c : newCollisions) {
        physics->resolveCollision(c);
    }
    enqueuePositions();
    enqueueCollisions(newCollisions);
}

void SweepNPruneAlgorithmSimple::insertionSort(std::vector<BoundingBox*> &boxes, int dimension)
{
    int i;
    int k;
    BoundingBox* temp;

    for (i = 1; i < (int)boxes.size(); i++)
    {
        k = i;
        while (k >= 1 && boxes[k]->getMin(dimension) < boxes[k-1]->getMin(dimension))
        {
            temp = boxes[k];
            boxes[k] = boxes[k-1];
            boxes[k-1] = temp;
            k--;
        }
    }
}
