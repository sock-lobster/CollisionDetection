#include "octree.h"
#include "constants.h"

Octree::Octree(int levelIn,
               double nwfXIn, double nwfYIn, double nwfZIn,
               double sebXIn, double sebYIn, double sebZIn)
{
    level = levelIn;
    maxParticles = MAX_PARTICLES_PER_LEVEL;
    maxLevels = MAX_LEVELS;
    nwfX = nwfXIn;
    nwfY = nwfYIn;
    nwfZ = nwfZIn;
    sebX = sebXIn;
    sebY = sebYIn;
    sebZ = sebZIn;
    for (int i = 0; i < 8; i++)
    {
        nodes[i] = NULL;
    }
}

Octree::~Octree()
{
    for (int i = 0; i < 8; i++)
    {
        if (nodes[0] != NULL)
        {
            delete nodes[i];
        }
    }
}

void Octree::clear()
{
    nodeParticles.clear();
    for (int i = 0; i < 8; i++)
    {
        if (nodes[0] != NULL)
        {
            nodes[i]->clear();
        }
    }
}

void Octree::split()
{
    double xMid = sebX / 2;
    double yMid = sebY / 2;
    double zMid = sebZ / 2;

    nodes[0] = new Octree(level+1, nwfX, nwfY, nwfZ, nwfX + xMid, nwfY + yMid, nwfZ + zMid);
    nodes[1] = new Octree(level+1, nwfX + xMid, nwfY, nwfZ, sebX, nwfY + yMid, nwfZ + zMid);
    nodes[2] = new Octree(level+1, nwfX, nwfY + yMid, nwfZ, nwfX + xMid, sebY, nwfZ + zMid);
    nodes[3] = new Octree(level+1, nwfX + xMid, nwfY + yMid, nwfZ, sebX, sebY, nwfZ + zMid);
    nodes[4] = new Octree(level+1, nwfX, nwfY, nwfZ + zMid, nwfX + xMid, nwfY + yMid, sebZ);
    nodes[5] = new Octree(level+1, nwfX + xMid, nwfY, nwfZ + zMid, sebX, nwfY + yMid, sebZ);
    nodes[6] = new Octree(level+1, nwfX, nwfY + yMid, nwfZ + zMid, nwfX + xMid, sebY, sebZ);
    nodes[7] = new Octree(level+1, nwfX + xMid, nwfY + yMid, nwfZ + zMid, sebX, sebY, sebZ);
}

int Octree::getIndex(Particle* particle)
{
    int index = -1;
    BoundingBox* box = new BoundingBox(particle);
    double xMid = sebX / 2;
    double yMid = sebY / 2;
    double zMid = sebZ / 2;

    double xMax = box->getXMax();
    double yMax = box->getYMax();
    double zMax = box->getZMax();
    double xMin = box->getXMin();
    double yMin = box->getYMin();
    double zMin = box->getZMin();

    // 0
    if (xMax <= xMid && yMax <= yMid && zMax <= zMid)
    {
        index = 0;
    }

    // 1
    else if (xMin > xMid && yMax <= yMid && zMax <= zMid)
    {
        index = 1;
    }

    // 2
    else if (xMax <= xMid && yMin > yMid && zMax <= zMid)
    {
        index = 2;
    }

    // 3
    else if (xMin > xMid && yMin > yMid && zMax <= zMid)
    {
        index = 3;
    }

    // 4
    else if (xMax <= xMid && yMax <= yMid && zMin > zMid)
    {
        index = 4;
    }

    // 5
    else if (xMin > xMid && yMax <= yMid && zMin > zMid)
    {
        index = 5;
    }

    // 6
    else if (xMax <= xMid && yMin > yMid && zMin > zMid)
    {
        index = 6;
    }

    // 7
    else if (xMin > xMid && yMin > yMid && zMin > zMid)
    {
        index = 7;
    }

    delete box;
    return index;
}

void Octree::insertToTree(Particle* particle)
{
    // particle belongs deeper in the tree
    if (nodes[0] != NULL)
    {
        int index = getIndex(particle);
        if (index != -1)
        {
            nodes[index]->insertToTree(particle);
            return;
        }
    }

    // particle belongs in this level of the tree
    nodeParticles.push_back(particle);

    // check that number of particles isnt over max allowed
    // if true, split required
    if ((int)nodeParticles.size() > maxParticles && level < maxLevels)
    {
        if (nodes[0] == NULL)
        {
            split();
        }

        // put particles into new nodes if possible
        for (int i = 0; i < (int)nodeParticles.size(); i++)
        {
            Particle* p = nodeParticles[i];
            int index = getIndex(p);
            if (index != -1)
            {
                nodes[index]->insertToTree(p);
                nodeParticles.erase(nodeParticles.begin() + i);
                i--;
            }
        }
    }
}

std::vector<Particle*>* Octree::getPossibleColliders(std::vector<Particle*>* colliders, Particle* p)
{
    int index = getIndex(p);
    if (index != -1 && nodes[0] != NULL)
    {
        nodes[index]->getPossibleColliders(colliders, p);
    }

    for (Particle* c : nodeParticles)
    {
        if (p != c)
        {
            colliders->push_back(c);
        }
    }
    return colliders;
}
