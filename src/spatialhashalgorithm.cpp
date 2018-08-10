#include "spatialhashalgorithm.h"
#include "constants.h"

SpatialHashAlgorithm::SpatialHashAlgorithm(double xIn, double yIn, int numParticles):
    Algorithm(xIn, yIn)
{
    createParticles2D(numParticles);

    // Implementing bounding boxes for use in this algorithm
    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }

    cellSize = MAX_PARTICLE_RADIUS*5;
}

SpatialHashAlgorithm::~SpatialHashAlgorithm() {}

int SpatialHashAlgorithm::hashFunc(int x, int y)
{
    // Looks like it is best to mod it by the closest prime to 10*numParticles
    // this is our way of estimating the closest prime to 10*numberOfParticles
    return (x*1640531513 ^ y*2654435789) % ((numParts*10) + 1);
}

void SpatialHashAlgorithm::calculateNextTimestep()
{
    timestep++;

    std::unordered_map<int, std::vector<Particle*> > particleMap ((numParts*10) + 1);
    std::unordered_map<int, std::vector<Particle*> >::iterator particleMapIt;
    std::unordered_set<int> collisionSet;

    for (int i = 0; i < numParts; i++)
    {
        physics->updatePositionAndVelocity(particles[i], timestep);

    }

    for (BoundingBox* box : boundingBoxes)
    {
        box->update();
    }

    std::unordered_set<int> cellKeys (4);

    std::vector<Particle*> particlesInCell;

    for (BoundingBox* box : boundingBoxes)
    {
        cellKeys.clear();
        particlesInCell.clear();

        Particle* q = box->getParticle();

        int xGridPos = box->getXMin()/cellSize;
        int yGridPos = box->getYMin()/cellSize;
        int edgeKey = hashFunc(xGridPos,yGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            if (particleMap.count(edgeKey) == 1)
            {
                particleMapIt = particleMap.find(edgeKey);
                ((*particleMapIt).second).push_back(q);
            }
            else
            {
                particlesInCell.push_back(q);
                cellKeys.insert(edgeKey);
                particleMap.insert(std::make_pair(edgeKey,particlesInCell));
            }
        }

        xGridPos = box->getXMin()/cellSize;
        yGridPos = box->getYMax()/cellSize;
        edgeKey = hashFunc(xGridPos,yGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            if (particleMap.count(edgeKey) == 1)
            {
                particleMapIt = particleMap.find(edgeKey);
                ((*particleMapIt).second).push_back(q);
            }
            else
            {
                particlesInCell.clear();
                particlesInCell.push_back(q);
                cellKeys.insert(edgeKey);
                particleMap.insert(std::make_pair(edgeKey,particlesInCell));
            }
        }

        xGridPos = box->getXMax()/cellSize;
        yGridPos = box->getYMin()/cellSize;
        edgeKey = hashFunc(xGridPos,yGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            if (particleMap.count(edgeKey) == 1)
            {
                particleMapIt = particleMap.find(edgeKey);
                ((*particleMapIt).second).push_back(q);
            }
            else
            {
                particlesInCell.clear();
                particlesInCell.push_back(q);
                cellKeys.insert(edgeKey);
                particleMap.insert(std::make_pair(edgeKey,particlesInCell));
            }
        }

        xGridPos = box->getXMax()/cellSize;
        yGridPos = box->getYMax()/cellSize;
        edgeKey = hashFunc(xGridPos,yGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            if (particleMap.count(edgeKey) == 1)
            {
                particleMapIt = particleMap.find(edgeKey);
                ((*particleMapIt).second).push_back(q);
            }
            else
            {
                particlesInCell.clear();
                particlesInCell.push_back(q);
                cellKeys.insert(edgeKey);
                particleMap.insert(std::make_pair(edgeKey,particlesInCell));
            }
        }
    }

    std::vector<Collision*> newCollisions;

    std::unordered_map<int,std::vector<Particle*> >::iterator iter;

    std::unordered_set<int>::iterator it;

    for (iter = particleMap.begin(); iter != particleMap.end(); iter++)
    {
        if ((*iter).second.size() > 1)
        {
            for (int k = 0; k < (int)(*iter).second.size(); k++)
            {
                for (int j=k+1; j < (int)(*iter).second.size(); j++)
                {
                    if ((*iter).second[k] != (*iter).second[j])
                    {
                        checks++;
                        if (physics->particlesOverlap((*iter).second[j], (*iter).second[k]))
                        {
                            int colliderKey = hashFunc((*iter).second[k]->getID(),(*iter).second[j]->getID());
                            int reverseColliderKey = hashFunc((*iter).second[j]->getID(),(*iter).second[k]->getID());
                            if (collisionSet.count(colliderKey) == 0 && collisionSet.count(reverseColliderKey) == 0 )
                            {
                                Collision* newCollision = physics->generateCollision((*iter).second[k],(*iter).second[j],timestep);
                                collisionSet.insert(colliderKey);
                                collisionSet.insert(reverseColliderKey);
                                newCollisions.push_back(newCollision);
                            }
                        }
                    }
                }
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
