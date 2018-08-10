#ifndef POSITION_H
#define POSITION_H
#include <valarray>
#include <iostream>
#include <queue>
#include <math.h>

/**
 * @brief The Position class holds location and time data for a Particle.
 */
class Position
{
public:
    /**
     * @brief Position constructor.
     *
     * Creates a Position object given x, y, and z components and a timestep.
     *
     * @param xIn x component.
     * @param yIn y component.
     * @param zIn z component.
     * @param timestepIn timestep of the Position.
     */
    Position(double xIn, double yIn, double zIn, int timestepIn);

    /**
     * @brief Position copy constructor.
     *
     * Creates a Position object by copying a Position.
     *
     * @param position pointer of the Particle to copy.
     */
    Position(Position* position);
    ~Position();

    /**
     * @brief Gets the x component of the Position.
     * @return x component of the Position.
     */
    double getX();

    /**
     * @brief Gets the y component of the Position.
     * @return y component of the Position.
     */
    double getY();

    /**
     * @brief Gets the z component of the Position.
     * @return z component of the Position.
     */
    double getZ();

    /**
     * @brief Gets the Position as a std::valarray<double>.
     * @return The Position as a std::valarray<double>.
     */
    std::valarray<double> getPos();

    /**
     * @brief Gets the timestep of the Position.
     * @return The timestep of the Position.
     */
    int getTimestep();

private:
    std::valarray<double> pos;
    int timestep;

};

#endif // POSITION_H
