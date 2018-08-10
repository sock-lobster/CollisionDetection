#include "quadtree.h"
#include "constants.h"

Quadtree::Quadtree(int levelIn,
                   double nwXIn, double nwYIn,
                   double seXIn, double seYIn)
{
    level = levelIn;
    maxParticles = MAX_PARTICLES_PER_LEVEL;
    maxLevels = MAX_LEVELS;
    nwX = nwXIn;
    nwY = nwYIn;
    seX = seXIn;
    seY = seYIn;
    for (int i = 0; i < 4; i++)
    {
        nodes[i] = NULL;
    }
}

Quadtree::~Quadtree()
{
    for (int i = 0; i < 4; i++)
    {
        if (nodes[0] != NULL)
        {
            delete nodes[i];
        }
    }
}

void Quadtree::clear()
{
    nodeParticles.clear();
    for (int i = 0; i < 4; i++)
    {
        if (nodes[0] != NULL)
        {
            nodes[i]->clear();
        }
    }
}

void Quadtree::split()
{
    double xMid = seX / 2;
    double yMid = seY / 2;

    nodes[0] = new Quadtree(level+1, nwX, nwY, nwX + xMid, nwY + yMid);
    nodes[1] = new Quadtree(level+1, nwX + xMid, nwY, seX, seY + yMid);
    nodes[2] = new Quadtree(level+1, nwX, nwY + yMid, nwX + xMid, nwY + yMid);
    nodes[3] = new Quadtree(level+1, nwX + xMid, nwY + yMid, seX, seY);
}

int Quadtree::getIndex(Particle* particle)
{
    int index = -1;
    BoundingBox* box = new BoundingBox(particle);
    double xMid = seX / 2;
    double yMid = seY / 2;

    double xMax = box->getXMax();
    double xMin = box->getXMin();
    double yMax = box->getYMax();
    double yMin = box->getYMin();

    //0
    if (xMax <= xMid && yMax <= yMid)
    {
        index = 0;
    }
    //1
    else if (xMin > xMid && yMax <= yMid)
    {
        index = 1;
    }
    //2
    else if (xMax <= xMid && yMin > yMid)
    {
        index = 2;
    }
    //3
    else if (xMin > xMid && yMin > yMid)
    {
        index = 3;
    }
    delete box;
    return index;
}

void Quadtree::insertToTree(Particle* particle)
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

std::vector<Particle*>* Quadtree::getPossibleColliders(std::vector<Particle*>* colliders, Particle* p)
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
