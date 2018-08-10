#ifndef OCTREE_H
#define OCTREE_H
#include "boundingbox.h"

/**
  * @brief The Octree data structure used in OctreeAlgorithm.
  *
  * A Octree represents a cubic section of space defined in the x, y, z coordinate system.
  **/
class Octree
{
public:
    /**
     * @brief Constructs a Octree object.
     *
     * Constructs a Octree object based on the given parameters.
     *
     * @param levelIn the level of the Octree object within the larger tree. The root of the tree is 0 and increases with each subsequent tree.
     * @param nwXIn the x coordinate of the upper left corner of the space represented by this level of the tree.
     * @param nwYIn the y coordinate of the upper left corner of the space represented by this level of the tree.
     * @param nwZIn the z coordinate of the upper left corner of the space represented by this level of the tree.
     * @param seXIn the x coordinate of the bottom right corner of the space represented by this level of the tree.
     * @param seYIn the y coordinate of the bottom right corner of the space represented by this level of the tree.
     * @param seZIn the z coordinate of the bottom right corner of the space represented by this level of the tree.
     */
    Octree(int levelIn,
           double nwfXIn, double nwfYIn, double nwfZIn,
           double sebXIn, double sebYIn, double sebZIn);
    ~Octree();

    /**
     * @brief Clears the Octree.
     *
     * Clears the Octree by recursively clearing nodeParticles at each level of the tree.
     */
    void clear();

    /**
     * @brief Splits the Octree into 8 octants.
     *
     * Splits the Octree into 8 octants and creates a new Octree for each octant.
     */
    void split();

    /**
     * @brief Gets the index of a given Particle within a Octree.
     *
     * Gets the index of a given Particle within a Octree. An index of -1 means the Particle cannot fit completely within any of the 8 nodes.
     * An index of 0, 1, 2, 3, 4, 5, 6, or 7 means the Particle fits completely within the cooresponding node.
     * @param particle pointer to the Particle to get an index of.
     * @return The index of a given Particle.
     */
    int getIndex(Particle* particle);

    /**
     * @brief Inserts a given Particle into a Octree.
     *
     * Inserts a given Particle into a Octree. If the Particle is inserted into a level of the Octree that has more Particle%s than is allowed
     * by MAX_PARTICLES_PER_LEVEL, Split() will be called on the node.
     * @param particle pointer to the Particle to insert into an Octree.
     */
    void insertToTree(Particle* particle);

    /**
     * @brief Returns a vector of Particle%s that may be colliding with a given Particle.
     *
     * Returns a pointer to the vector of pointers to Particle%s that may be colliding with a given Particle.
     * @param colliders pointer to the vector of Particle%s that may be colliding with a given Particle.
     * @param p pointer to the given Particle to check against.
     * @return Pointer to a vector of pointers to Particle%s.
     */
    std::vector<Particle*>* getPossibleColliders(std::vector<Particle*> *colliders, Particle* particle);

private:
    int maxParticles;
    int maxLevels;
    int level;
    std::vector<Particle*> nodeParticles;
    double nwfX;
    double nwfY;
    double nwfZ;
    double sebX;
    double sebY;
    double sebZ;
    Octree* nodes [8];
};

#endif // OCTREE_H
