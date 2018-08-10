#ifndef PHYSICS3D_H
#define PHYSICS3D_H
#include "physics.h"

/**
 * @brief The Physics3D class is a Physics that works specifically in 3D.
 *
 * This Physics is designed for calculating Particle Collision%s and Position
 * updates in 3-space, but not with gravity.
 */
class Physics3D : public Physics
{
public:
    /**
     * @brief Constructs a Physics3D object.
     *
     * This calls Physics::Physics(), then initializes the size of the space.
     *
     * @param xWinSize the size of the x dimension of this space.
     * @param yWinSize the size of the y dimension of this space.
     * @param zWinSize the size of the z dimension of this space.
     */
    Physics3D(double xIn, double yIn, double zIn);
    ~Physics3D();

private:
    void fixWallCollision(Particle* p, double newXPos, double newYpos, double newZpos, double newXvel, double newYvel, double newZvel, int timestep);
    void updatePositionAndVelocity(Particle* p, int timestep);

    double xWinSize;
    double yWinSize;
    double zWinSize;
};

#endif // PHYSICS3D_H
