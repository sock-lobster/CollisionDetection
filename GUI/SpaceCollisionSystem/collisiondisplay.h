#ifndef COLLISIONDISPLAY_H
#define COLLISIONDISPLAY_H
#include <QTimer>
#include "qalgorithm.h"

/**
 * @brief The parent class of 2D and 3D visualization windows.
 *
 * CollisionDisplay defines variables and methods necessary for both 2D and 3D
 * visualization as well as the slots and signals that must be overwritten by
 * its subclasses.
 */
class CollisionDisplay
{
public:
    /**
     * @brief Constructs a CollisionDisplay.
     *
     * Initializes the timer, the number of collisions seen, and the display
     * timestep.
     */
    explicit CollisionDisplay();
    virtual ~CollisionDisplay();

    /**
     * @brief Returns a pointer to the timer.
     *
     * Returns a pointer to the timer which is responsible for maintaining the
     * framerate of the display.
     *
     * @return A pointer to the timer.
     */
    QTimer* getTimer();

    /**
     * @brief Returns the number of Collision%s seen so far.
     * @return The number of Collision%s seen.
     */
    int getNumCollisions();

    /**
     * @brief Gives a pointer to the QAlgorithm to display.
     *
     * Sets the QAlgorithm pointer for the display so the display can access
     * the Position and Collision queues.
     *
     * @param qalgorithm a pointer to the QAlgorithm.
     */
    void setQAlgorithm(QAlgorithm* qalgorithm);

    /**
     * @brief Returns the display timestep.
     * @return The display timestep.
     */
    int getDisplayTimestep();

signals:
    /**
     * @brief Signals that the display is no longer running.
     *
     * Signals that the display has finished running so it is safe to run a new
     * simulation or close the program.
     */
    virtual void finished() = 0;

public slots:
    /**
     * @brief Slot used to initialize the display.
     *
     * A purely virtual method which should be used for initilization tasks,
     * such as building a list of drawn Particle%s and reseting values.
     */
    virtual void start() = 0;

    /**
     * @brief Slot used to stop the display.
     *
     * A purely virtual method which should be used to stop and clean up the
     * display. In particular, the timer should be stopped, and the finished()
     * signal should be emitted.
     */
    virtual void stop() = 0;

    /**
     * @brief Slot used to step the display.
     *
     * A purely virtual method which should be used to move the display forward
     * one step. For example, drawn Particle Position%s and Collision
     * information should be updated.
     */
    virtual void step() = 0;

protected:
    QTimer* timer;
    QAlgorithm* qalg;
    int numCollisions;
    int displayTimestep;
    MultiQueue* positionQueue;
    std::queue<int> collisionTimesteps;
};

#endif // COLLISIONDISPLAY_H
