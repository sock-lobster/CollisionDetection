#include "spatialhashalgorithm3d.h"
#include "constants.h"

SpatialHashAlgorithm3D::SpatialHashAlgorithm3D(double xIn, double yIn, double zIn, int numParticles):
    Algorithm(xIn, yIn, zIn)
{
    createParticles3D(numParticles);

    // Implementing bounding boxes for use in this algorithm
    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }

    cellSize = MAX_PARTICLE_RADIUS * 5;
}

SpatialHashAlgorithm3D::SpatialHashAlgorithm3D(double xIn, double yIn, double zIn, int numParticles, double centerMass):
    Algorithm(xIn, yIn, zIn, centerMass)
{
    createParticlesOrbit(numParticles);

    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }

    cellSize = MAX_PARTICLE_RADIUS * 5;
}

int SpatialHashAlgorithm3D::hashFunc(int x, int y, int z)
{
    // Looks like it is best to mod it by the closest prime to 10*numParticles
    // this is our way of estimating the closest prime to 10*numberOfParticles
    return (x*73856093 ^ y*19349663 ^ z*83492791) % ((numParts*10) + 1);
}

int SpatialHashAlgorithm3D::hashFunc(int x, int y)
{
    // Looks like it is best to mod it by the closest prime to 10*numParticles
    // this is our way of estimating the closest prime to 10*numberOfParticles
    return (x*73856093 ^ y*19349663) % ((numParts*10) + 1);
}

SpatialHashAlgorithm3D::~SpatialHashAlgorithm3D() {}

void SpatialHashAlgorithm3D::calculateNextTimestep()
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
        int zGridPos = box->getZMin()/cellSize;
        int edgeKey = hashFunc(xGridPos,yGridPos,zGridPos);

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
        zGridPos = box->getZMin()/cellSize;
        edgeKey = hashFunc(xGridPos,yGridPos,zGridPos);

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
        zGridPos = box->getZMin()/cellSize;
        edgeKey = hashFunc(xGridPos,yGridPos,zGridPos);

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
        zGridPos = box->getZMax()/cellSize;
        edgeKey = hashFunc(xGridPos,yGridPos,zGridPos);

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
        zGridPos = box->getZMin()/cellSize;
        edgeKey = hashFunc(xGridPos,yGridPos,zGridPos);

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

        xGridPos = box->getXMin()/cellSize;
        yGridPos = box->getYMax()/cellSize;
        zGridPos = box->getZMax()/cellSize;
        edgeKey = hashFunc(xGridPos,yGridPos,zGridPos);

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

        xGridPos = box->getXMin()/cellSize;
        yGridPos = box->getYMin()/cellSize;
        zGridPos = box->getZMax()/cellSize;
        edgeKey = hashFunc(xGridPos,yGridPos,zGridPos);

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
        zGridPos = box->getZMax()/cellSize;
        edgeKey = hashFunc(xGridPos,yGridPos,zGridPos);

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
                        if (physics->particlesOverlap((*iter).second[k], (*iter).second[j]))
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
