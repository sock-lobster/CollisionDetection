#include "bruteforcealgorithm.h"
#include "constants.h"

BruteForceAlgorithm::BruteForceAlgorithm(double xIn, double yIn, int numParticles):
    Algorithm(xIn, yIn)
{
    createParticles2D(numParticles);
}

BruteForceAlgorithm::BruteForceAlgorithm(double xIn, double yIn, double zIn, int numParticles):
    Algorithm(xIn, yIn, zIn)
{
    createParticles3D(numParticles);
}

BruteForceAlgorithm::BruteForceAlgorithm(double xIn, double yIn, double zIn, int numParticles, double centerMass):
    Algorithm(xIn, yIn, zIn, centerMass)
{
    createParticlesOrbit(numParticles);
}

BruteForceAlgorithm::~BruteForceAlgorithm()
{}

void BruteForceAlgorithm::calculateNextTimestep()
{
    timestep++;

    for (int i = 0; i < numParts; i++)
    {
        physics->updatePositionAndVelocity(particles[i], timestep);
    }

    std::vector<Collision*> newCollisions;

    for (int i = 0; i < (int)particles.size(); i++)
    {
        Particle* a = particles[i];
        for (int j = i+1; j < (int)particles.size(); j++)
        {
            checks++;
            Particle* b = particles[j];
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
