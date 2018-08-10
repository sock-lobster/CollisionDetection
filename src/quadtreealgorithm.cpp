#include "quadtreealgorithm.h"
#include "constants.h"

#include <time.h>
#include <unordered_map>

QuadtreeAlgorithm::QuadtreeAlgorithm(double xIn, double yIn, int numParticlesIn):
    Algorithm(xIn, yIn)
{
    MAX_PARTICLES_PER_LEVEL = 5;
    MAX_LEVELS = numParticlesIn / MAX_PARTICLES_PER_LEVEL;

    createParticles2D(numParticlesIn);

    quadtree = new Quadtree(0, 0, 0, xIn, yIn);
    for (Particle* p : particles)
    {
        quadtree->insertToTree(p);
    }
}

QuadtreeAlgorithm::~QuadtreeAlgorithm()
{
    delete quadtree;
}

int QuadtreeAlgorithm::hash(int a, int b)
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

void QuadtreeAlgorithm::calculateNextTimestep()
{
    timestep++;

    for (int i = 0; i < numParts; i++)
    {
        physics->updatePositionAndVelocity(particles[i], timestep);
    }

    // clear quadtree
    quadtree->clear();

    // insert all particles into quadtree
    for (Particle* p : particles)
    {
        quadtree->insertToTree(p);
    }

    // for each particle get list of possible colliders and check for collisions
    std::vector<Particle*>* colliders = new std::vector<Particle*>;
    std::vector<Collision*> newCollisions;
    std::unordered_map<int, Collision*> uniqueCollisions;

    for (Particle* p : particles)
    {
        colliders->clear();
        colliders = quadtree->getPossibleColliders(colliders, p);

        for (Particle* c : *colliders)
        {
            checks++;
            int h = hash(p->getID(), c->getID());
            if (!uniqueCollisions.count(h) && physics->particlesOverlap(p, c))
            {
                Collision* newCollision = physics->generateCollision(p, c, timestep);
                newCollisions.push_back(newCollision);
                uniqueCollisions[h] = newCollision;
            }
        }
    }
    std::sort(newCollisions.begin(), newCollisions.end());
    for (Collision* c : newCollisions) {
        physics->resolveCollision(c);
    }
    enqueuePositions();
    enqueueCollisions(newCollisions);
    delete colliders;
}
