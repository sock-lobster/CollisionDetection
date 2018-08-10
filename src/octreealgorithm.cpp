#include "octreealgorithm.h"
#include <unordered_map>

OctreeAlgorithm::OctreeAlgorithm(double xIn, double yIn, double zIn, int numParticlesIn):
    Algorithm(xIn, yIn, zIn)
{
    MAX_PARTICLES_PER_LEVEL = 5;
    MAX_LEVELS = numParticlesIn / MAX_PARTICLES_PER_LEVEL;

    createParticles3D(numParticlesIn);

    octree = new Octree(0, 0, 0, 0, xIn, yIn, zIn);
    for (Particle* p : particles)
    {
        octree->insertToTree(p);
    }
}

OctreeAlgorithm::OctreeAlgorithm(double xIn, double yIn, double zIn, double centerMassIn, int numParticlesIn):
    Algorithm(xIn, yIn, zIn, centerMassIn)
{
    MAX_PARTICLES_PER_LEVEL = 5;
    MAX_LEVELS = numParticlesIn / MAX_PARTICLES_PER_LEVEL;

    createParticlesOrbit(numParticlesIn);

    octree = new Octree(0, 0, 0, 0, xIn, yIn, zIn);
    for (Particle* p : particles)
    {
        octree->insertToTree(p);
    }
}

OctreeAlgorithm::~OctreeAlgorithm()
{
    delete octree;
}

int OctreeAlgorithm::hash(int a, int b)
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


void OctreeAlgorithm::calculateNextTimestep()
{
    timestep++;

    for (int i = 0; i < numParts; i++)
    {
        physics->updatePositionAndVelocity(particles[i], timestep);
    }

    // clear octree
    octree->clear();

    // insert all particles into octree
    for (Particle* p : particles)
    {
        octree->insertToTree(p);
    }

    // for each particle get list of possible colliders and check for collisions
    std::vector<Particle*>* colliders = new std::vector<Particle*>;
    std::vector<Collision*> newCollisions;
    std::unordered_map<int, Collision*> uniqueCollisions;

    for (Particle* p : particles)
    {
        colliders->clear();
        colliders = octree->getPossibleColliders(colliders, p);

        // collision check
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
