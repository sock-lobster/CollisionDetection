#include "spatialindexalgorithm3d.h"
#include "constants.h"

SpatialIndexAlgorithm3D::SpatialIndexAlgorithm3D(double xIn, double yIn, double zIn, int numParticles):
    Algorithm(xIn, yIn, zIn)
{
    createParticles3D(numParticles);

    // Implementing bounding boxes for use in this algorithm
    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }

    cellSize = MAX_PARTICLE_RADIUS*5;
}

SpatialIndexAlgorithm3D::SpatialIndexAlgorithm3D(double xIn, double yIn, double zIn, int numParticles, double centerMass):
    Algorithm(xIn, yIn, zIn, centerMass)
{
    createParticlesOrbit(numParticles);

    // Implementing bounding boxes for use in this algorithm
    for (Particle* p : particles)
    {
        boundingBoxes.push_back(new BoundingBox(p));
    }

    cellSize = MAX_PARTICLE_RADIUS*5;
}

SpatialIndexAlgorithm3D::~SpatialIndexAlgorithm3D()
{
    while (!particles.empty())
    {
        Particle* p = particles.back();
        delete p;
        particles.pop_back();
    }
}

int SpatialIndexAlgorithm3D::hashFunc(int x, int y)
{
    // Looks like it is best to mod it by the closest prime to 10*numParticles
    // this is our way of estimating the closest prime to 10*numberOfParticles
    return (x*1640531513 ^ y*2654435789) % ((numParts*10) + 1);
}

int SpatialIndexAlgorithm3D::hashFunc(int x, int y, int z)
{
    // Looks like it is best to mod it by the closest prime to 10*numParticles
    // this is our way of estimating the closest prime to 10*numberOfParticles
    return (x*73856093 ^ y*19349663 ^ z*83492791) % ((numParts*10) + 1);
}

void SpatialIndexAlgorithm3D::calculateNextTimestep()
{
    timestep++;

    int rowSize = ceil(X_DIMENSION_SIZE/cellSize);
    std::vector<std::vector<Particle*> > cellGrid (pow(rowSize,3));
    std::unordered_set<int> collisionSet;

    for (int i = 0; i < numParts; i++)
    {
        physics->updatePositionAndVelocity(particles[i], timestep);

    }

    for (BoundingBox* box : boundingBoxes)
    {
        box->update();
    }

    std::unordered_set<int> cellKeys (8);

    for (BoundingBox* box : boundingBoxes)
    {
        cellKeys.clear();

        Particle* q = box->getParticle();

        int xGridPos = box->getXMin()/cellSize;
        int yGridPos = box->getYMin()/cellSize;
        int zGridPos = box->getZMin()/cellSize;
        int cellIndex = (xGridPos*rowSize*rowSize)+yGridPos*rowSize+zGridPos;
        int edgeKey = hashFunc(xGridPos,yGridPos, zGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }

        xGridPos = box->getXMin()/cellSize;
        yGridPos = box->getYMax()/cellSize;
        zGridPos = box->getZMin()/cellSize;
        cellIndex = (xGridPos*rowSize*rowSize)+yGridPos*rowSize+zGridPos;
        edgeKey = hashFunc(xGridPos,yGridPos, zGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }

        xGridPos = box->getXMax()/cellSize;
        yGridPos = box->getYMin()/cellSize;
        zGridPos = box->getZMax()/cellSize;
        cellIndex = (xGridPos*rowSize*rowSize)+yGridPos*rowSize+zGridPos;
        edgeKey = hashFunc(xGridPos,yGridPos, zGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }

        xGridPos = box->getXMax()/cellSize;
        yGridPos = box->getYMax()/cellSize;
        zGridPos = box->getZMin()/cellSize;
        cellIndex = (xGridPos*rowSize*rowSize)+yGridPos*rowSize+zGridPos;
        edgeKey = hashFunc(xGridPos,yGridPos, zGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }

        xGridPos = box->getXMax()/cellSize;
        yGridPos = box->getYMax()/cellSize;
        zGridPos = box->getZMax()/cellSize;
        cellIndex = (xGridPos*rowSize*rowSize)+yGridPos*rowSize+zGridPos;
        edgeKey = hashFunc(xGridPos,yGridPos, zGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }

        xGridPos = box->getXMax()/cellSize;
        yGridPos = box->getYMin()/cellSize;
        zGridPos = box->getZMin()/cellSize;
        cellIndex = (xGridPos*rowSize*rowSize)+yGridPos*rowSize+zGridPos;
        edgeKey = hashFunc(xGridPos,yGridPos, zGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }

        xGridPos = box->getXMin()/cellSize;
        yGridPos = box->getYMax()/cellSize;
        zGridPos = box->getZMax()/cellSize;
        cellIndex = (xGridPos*rowSize*rowSize)+yGridPos*rowSize+zGridPos;
        edgeKey = hashFunc(xGridPos,yGridPos, zGridPos);

        if (cellKeys.count(edgeKey) == 0)
        {
            cellGrid[cellIndex].push_back(q);
            cellKeys.insert(edgeKey);
        }

        xGridPos = box->getXMin()/cellSize;
        yGridPos = box->getYMin()/cellSize;
        zGridPos = box->getZMax()/cellSize;
        cellIndex = (xGridPos*rowSize*rowSize)+yGridPos*rowSize+zGridPos;
        edgeKey = hashFunc(xGridPos,yGridPos, zGridPos);

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
