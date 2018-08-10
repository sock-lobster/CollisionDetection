#include "spatialindexalgorithm.h"

#include "spatialhashalgorithm.h"
#include "constants.h"

SpatialIndexAlgorithm::SpatialIndexAlgorithm(double xIn, double yIn, int numParticles):
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

SpatialIndexAlgorithm::~SpatialIndexAlgorithm() {}

int SpatialIndexAlgorithm::hashFunc(int x, int y)
{
    // Looks like it is best to mod it by the closest prime to 10*numParticles
    // this is our way of estimating the closest prime to 10*numberOfParticles
    return (x*1640531513 ^ y*2654435789) % ((numParts*10) + 1);
}

int SpatialIndexAlgorithm::hashFunc(int x, int y, int z)
{
    // Looks like it is best to mod it by the closest prime to 10*numParticles
    // this is our way of estimating the closest prime to 10*numberOfParticles
    return (x*73856093 ^ y*19349663 ^ z*83492791) % ((numParts*10) + 1);
}

void SpatialIndexAlgorithm::calculateNextTimestep()
{
    timestep++;

    int rowSize = ceil(X_DIMENSION_SIZE/cellSize);
    std::vector<std::vector<Particle*> > cellGrid (pow(rowSize,2));
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

    for (BoundingBox* box : boundingBoxes)
    {
        cellKeys.clear();

        Particle* q = box->getParticle();

        int xGridPos = box->getXMin()/cellSize;
        int yGridPos = box->getYMin()/cellSize;
        int cellIndex = (xGridPos*rowSize)+yGridPos;
        int edgeKey = hashFunc(xGridPos,yGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }

        xGridPos = box->getXMin()/cellSize;
        yGridPos = box->getYMax()/cellSize;
        cellIndex = (xGridPos*rowSize)+yGridPos;
        edgeKey = hashFunc(xGridPos,yGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }

        xGridPos = box->getXMax()/cellSize;
        yGridPos = box->getYMin()/cellSize;
        cellIndex = (xGridPos*rowSize)+yGridPos;
        edgeKey = hashFunc(xGridPos,yGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }

        xGridPos = box->getXMax()/cellSize;
        yGridPos = box->getYMax()/cellSize;
        cellIndex = (xGridPos*rowSize)+yGridPos;
        edgeKey = hashFunc(xGridPos,yGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }
    }

    std::vector<Collision*> newCollisions;

    for (int x = 0; x < (int)cellGrid.size(); x++)
    {
        if (cellGrid[x].size() > 1)
        {
            std::vector<Particle*> particlesInCell = cellGrid[x];
            for (int i=0; i<(int)particlesInCell.size();i++)
            {
                Particle* a = particlesInCell[i];
                for (int j =i+1; j<(int)particlesInCell.size(); j++)
                {
                    Particle* b = particlesInCell[j];
                    checks++;
                    if (physics->particlesOverlap(a,b))
                    {
                        int colliderKey = hashFunc(a->getID(),b->getID());
                        int reverseColliderKey = hashFunc(b->getID(),a->getID());
                        if (collisionSet.count(colliderKey) == 0 && collisionSet.count(reverseColliderKey) == 0 )
                        {
                            Collision* newCollision = physics->generateCollision(a, b, timestep);
                            collisionSet.insert(colliderKey);
                            collisionSet.insert(reverseColliderKey);
                            newCollisions.push_back(newCollision);
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
