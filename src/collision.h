#ifndef COLLISION_H
#define COLLISION_H
#include "particle.h"

class Particle;

/**
 * @brief The Collision class is an object representing two Particle%s hitting.
 */

class Collision
{
public:
    /**
     * @brief Constructs a Collision object for given Particle%s and time data.
     * @param partA One of the Particle%s involved in the Collision.
     * @param partB The other Particle involved in the Collision.
     * @param timestepIn The timestep at which the particles are overlapping.
     * @param overlapIn The amount of continuous time the Particle%s overlap
     * before the end of the timestep where the Collision took place.
     *
     */
    Collision(Particle* partA, Particle* partB, int timestepIn, double overlapIn);

    /**
     * @brief Returns one of the Particle%s involved in the Collision.
     * @return A Particle involved in this Collision.
     */
    Particle* getColliderA();

    /**
     * @brief Returns one of the Particle%s involved in the Collision.
     * @return A Particle involved in this Collision.
     */
    Particle* getColliderB();

    /**
     * @brief Returns the timestep when these Particle%s hit eachother.
     * @return the timestep of the Collision
     */
    int getCollisionTimestep();

    /**
     * @brief Returns the amount of continuous time the Particle%s are
     * overlapped durring the timestep that they collide.
     * @return the continuous overlap time.
     */
    double getOverlapTime();

    /**
     * @brief Returns true if this Collision's overlap time is longer than the
     * overlap time of the Collision we are comparing to.
     *
     * This is counter-intuitive, but it allows us to use the built in sorting
     * and end up with a sorted list of collisions where those with the longest
     * overlap times come first (because those get bounced first).
     *
     * @param other The Collision we are comparing this Collision to.
     * @return Whether or not this collision has a longer overlap time.
     */
    bool operator<(Collision* other);

private:
    Particle* a;
    Particle* b;
    int timestep;
    double overlapTime;
};

#endif // COLLISION_H
