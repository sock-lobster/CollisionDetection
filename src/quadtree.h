#ifndef QUADTREE_H
#define QUADTREE_H
#include "boundingbox.h"

/**
  * @brief The Quadtree data structure used in QuadtreeAlgorithm.
  *
  * A Quadtree represents a rectangular section of space defined in the x, y coordinate system.
  **/
class Quadtree
{
public:
    /**
     * @brief Constructs a Quadtree object.
     *
     * Constructs a Quadtree object based on the given parameters.
     *
     * @param levelIn the level of the Quadtree object within the larger tree. The root of the tree is 0 and increases from there.
     * @param nwXIn the x coordinate of the upper left corner of the space represented by this level of the tree.
     * @param nwYIn the y coordinate of the upper left corner of the space represented by this level of the tree.
     * @param seXIn the x coordinate of the bottom right corner of the space represented by this level of the tree.
     * @param seYIn the y coordinate of the bottom right corner of the space represented by this level of the tree.
     */
    Quadtree(int levelIn,
             double nwXIn, double nwYIn,
             double seXIn, double seYIn);
    ~Quadtree();

    /**
     * @brief Clears the Quadtree.
     *
     * Clears the Quadtree by recursively clearing nodeParticles at each level of the tree.
     */
    void clear();

    /**
     * @brief Splits the Quadtree into 4 quadrants.
     *
     * Splits the Quadtree into 4 quadrants and creates a new Quadtree for each quadrant.
     */
    void split();

    /**
     * @brief Gets the index of a given Particle within a Quadtree.
     *
     * Gets the index of a given Particle within a Quadtree. An index of -1 means the Particle cannot fit completely within any of the 4 nodes.
     * An index of 0, 1, 2, or 3 means the Particle fits completely within the cooresponding node.
     * @param particle pointer to the Particle to get an index of.
     * @return Index of a given Particle.
     */
    int getIndex(Particle* particle);

    /**
     * @brief Inserts a given Particle into a Quadtree.
     *
     * Inserts a given Particle into a Quadtree. If the Particle is inserted into a level of the Quadtree that has more Particle%s than is allowed
     * by MAX_PARTICLES_PER_LEVEL, Split() will be called on the node.
     * @param particle pointer to the Particle to insert into a Quadtree.
     */
    void insertToTree(Particle* particle);

    /**
     * @brief Returns a vector of Particle%s that may be colliding with a given Particle.
     *
     * Returns a pointer to a vector of pointers to Particle%s that may be colliding with a given Particle.
     * @param colliders pointer to the vector of Particle%s that may be colliding with a given Particle.
     * @param p pointer to the given Particle to check against.
     * @return A pointer to the vector of pointers to Particle%s.
     */
    std::vector<Particle*>* getPossibleColliders(std::vector<Particle *> *colliders, Particle* p);


private:
    int maxParticles;
    int maxLevels;
    int level;
    std::vector<Particle*> nodeParticles;
    double nwX;
    double nwY;
    double seX;
    double seY;
    Quadtree* nodes [4];

};

#endif // QUADTREE_H
