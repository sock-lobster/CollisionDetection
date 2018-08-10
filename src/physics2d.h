#ifndef PHYSICS2D_H
#define PHYSICS2D_H
#include "physics.h"

/**
 * @brief The Physics2D class is a Physics that works specifically in 2D.
 *
 * This is useful because it simplifies calculations especially related to
 * checking if Particle%s are bouncing off the walls of the space because it
 * doesn't have to worry about the z dimension.
 */
class Physics2D : public Physics
{
public:
    /**
     * @brief Constructs a Physics2D object.
     *
     * This calls Physics::Physics(), then initializes the size of the space.
     *
     * @param xWinSize the size of the x dimension of this space.
     * @param yWinSize the size of the y dimension of this space.
     */
    Physics2D(double xWinSize, double yWinSize);
    ~Physics2D();

protected:
    double xWinSize;
    double yWinSize;

    void fixWallCollision(Particle* p, double newXpos, double newYpos, double newXvel, double newYvel, int timestep);

private:
    void updatePositionAndVelocity(Particle* p, int timestep);
};

#endif // PHYSICS2D_H
