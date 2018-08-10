#ifndef PARTICLE_H
#define PARTICLE_H
#include <iostream>
#include "position.h"
#include "collision.h"
#include "node.h"

class Collision;
class Node;
/**
 * @brief The Particle class
 *
 * Contains information about the Particle including its ID, Position, velocity, radius, and mass.
 * Also has accessors to a Particle's queue of future Position%s in the MultiQueue.
 */
class Particle
{
public:
    /**
     * @brief Particle constructor.
     * @param id ID of the Particle.
     * @param startPos pointer to the starting Position of the Particle.
     * @param startVel valarray of the starting velocity of the Particle.
     * @param radiusIn radius of the Particle.
     * @param massIn mass of the Particle.
     */
    Particle(int id, Position* startPos, std::valarray<double> startVel, double radiusIn, double massIn);
    ~Particle();

    /**
     * @brief Gets the ID of the Particle.
     * @return ID of the Particle.
     */
    int getID();

    /**
     * @brief Gets the x velocity of the Particle.
     * @return x velocity of the Particle.
     */
    double getXvel();

    /**
     * @brief Gets the y velocity of the Particle.
     * @return y velocity of the Particle.
     */
    double getYvel();

    /**
     * @brief Gets the z velocity of the Particle.
     * @return z velocity of the Particle.
     */
    double getZvel();

    /**
     * @brief Gets the x Position of the Particle.
     * @return x Position of the Particle.
     */
    double getXpos();

    /**
     * @brief Gets the y Position of the Particle.
     * @return y Position of the Particle.
     */
    double getYpos();

    /**
     * @brief Gets the z Position of the Particle.
     * @return z Position of the Particle.
     */
    double getZpos();

    /**
     * @brief Gets the mass of the Particle.
     * @return Mass of the Particle.
     */
    double getMass();

    /**
     * @brief Gets the radius of the Particle.
     * @return Radius of the Particle.
     */
    double getRadius();

    /**
     * @brief Gets the current Position of the Particle.
     * @return Pointer to the current Position of the Particle.
     */
    Position* getCurPos();

    /**
     * @brief Returns the first instance of this Particle in the MultiQueue.
     * @return The first instance of this Particle in the MultiQueue.
     */
    Node* getHead();

    /**
     * @brief Returns the last instance of this Particle in the MultiQueue.
     * @return The last instance of this Particle in the MultiQueue.
     */
    Node* getTail();

    /**
     * @brief Sets the Particle's head instance in the MultiQueue.
     * @param n the Node containing the Position that is being pushed on the
     * head of this Particle's queue within the MultiQueue.
     */
    void setHead(Node* n);

    /**
     * @brief Sets the Particle's tail instance in the MultiQueue.
     * @param n the Node containing the Position that is being pushed on the
     * tail of this Particle's queue within the MultiQueue.
     */
    void setTail(Node* n);

    /**
     * @brief Gets the velocity of the Particle.
     * @return Velocity of the Particle in a std::valarray<double>.
     */
    std::valarray<double> getVel();

    /**
     * @brief Updates the velocity of the Particle.
     * @param x x component of the velocity.
     * @param y y component of the velocity.
     * @param z z component of the velocity.
     */
    void updateVel(double x, double y, double z);

    /**
     * @brief Updates the Position of the Particle.
     * @param Pointer to the new Position of the Particle.
     */
    void updateCurPos(Position* newPos);

    /**
     * @brief Gets the acceleration of the Particle.
     * @return Acceleration of the Particle in a std::valarray<double>.
     */
    std::valarray<double> getAcceleration();

    /**
     * @brief Updates the acceleration of the Particle.
     * @param newA new acceleration of the Particle.
     */
    void updateAcceleration(std::valarray<double> newA);

private:
    int id;
    double radius;
    double mass;

    Node* head;
    Node* tail;

    std::valarray<double> vel;
    std::valarray<double> acceleration;
    Position* curPos;
};

#endif // PARTICLE_H
