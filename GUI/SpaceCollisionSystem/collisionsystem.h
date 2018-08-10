#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H
#include <QMainWindow>
#include <QThread>
#include <QCloseEvent>
#include <QShortcut>
#include "qalgorithm.h"
#include "collisiondisplay3d.h"

namespace Ui {
class CollisionSystem;
}

/**
 * @brief The main window of the graphical interface.
 *
 * CollisionSystem is the graphical user interface as well as the bridge
 * between an Algoithm and the CollisionDisplay objects (2D and 3D).
 */
class CollisionSystem : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a CollisionSystem object.
     *
     * Constructs a CollisionSystem object, initializing the elements of the
     * GUI and connecting signals and slots that should remain connected for
     * the duration of the program.
     *
     * @param parent the parent widget, if one exists. Defaults to 0.
     */
    explicit CollisionSystem(QWidget *parent = 0);
    ~CollisionSystem();

    /**
     * @brief Sets the seed for the random number generator.
     *
     * Sets the value of SEED based on the value in the corresponding GUI
     * input field.
     */
    void setSeed();
    /**
     * @brief Sets the number of Particle%s with which an Algorithm will run.
     *
     * Change the value of NUM_PARTICLES to be the value in the Particle number
     * input field. Called when the value changes.
     */
    void setNumParticles();

    /**
     * @brief Sets the range for Particle radius.
     *
     * Set the values of MIN_PARTICLE_RADIUS and MAX_PARTICLE_RADIUS to match
     * the values in the min and max radius input field.
     */
    void setRadiusRange();

private:
    void initializeConnections();
    void closeEvent(QCloseEvent* event);

    Ui::CollisionSystem *ui;
    QAlgorithm* qalg;
    QThread* cdThread;
    QThread* algThread;
    QTimer outputTimer;

    QShortcut* pauseShortcut;
    QShortcut* stepShortcut;
    QShortcut* startShortcut;
    QShortcut* stopShortcut;

    bool paused;
    bool algRunning;
    bool displayRunning;
    int environment;

    CollisionDisplay3D* collisionDisplay3D;
    QWidget* display3DContainer;

    QCloseEvent* closing;

public slots:
    /**
     * @brief Creates an Algorithm and sets up connections to the GUI.
     *
     * Uses the current parameters set in the GUI to create a new Algorithm
     * object. Connects GUI input to the Algorithm and appropriate display, and
     * begins the Algorithm and display. Called when the Start button is
     * clicked.
     */
    void start();

    /**
     * @brief Kills the Algorithm thread and cleans up for the next Algorithm.
     *
     * Stops the Algorithm thread, destroys the Algorithm object, and
     * disconnects GUI input elements to prepare for the next time Start is
     * clicked. Only runs if both the Algorithm and display have finished
     * (emitting the finsihed() signal).
     */
    void terminate();

    /**
     * @brief Prints selected information to the GUI text output.
     *
     * Clears and rewrites certain information to the text output field in the
     * GUI. The information written depends on the items chosen by the user in
     * the view menu. By default, the current timestep and the number of
     * Collision%s are displayed.
     */
    void printInfo();

    /**
     * @brief Pauses or unpauses the display.
     *
     * Stops a CollisionDisplay's timer so that the display effectively pauses.
     * If pausing, enables the Step button and changes the label of the paused
     * button to "Play." If unpausing (playing), disables the Step button and
     * changes the label back to "Pause." Called when the Pause button is
     * clicked.
     *
     * Note that this does not pause the Algorithm; it will continue running
     * until it fills the Position queue.
     */
    void changePausedState();

    /**
     * @brief Moves the camera back to the starting position (for a 3D display
     * only).
     *
     * Changes the location of the camera and the up direction so that the
     * dispaly returns to the original view location. Called when the Reset
     * Camera button is clicked.
     */
    void resetCamera();

    /**
     * @brief Generates a new seed for the random number generator.
     *
     * Uses the system time to create a new value for SEED, sets the value of
     * the input field to the new seed. Does not actually change the value of
     * SEED. Called when the Generate Seed button is clicked.
     * @sa setSeed()
     */
    void generateSeed();

    /**
     * @brief Tells the CollisionSystem the the Algorithm is no longer running.
     *
     * Sets the corresponding boolean so that the CollisionSystem can consider
     * the Algorithm as finished when calling terminate(). Called when an
     * QAlgorithm emits the QAlgorithm::finished() signal.
     */
    void setAlgFinished();

    /**
     * @brief Tells the CollisionSystem that the display is no longer running.
     *
     * Sets the corresponding boolean so that the CollisionSystem can consider
     * the display as finished when calling terminate(). Called when a
     * CollisionDisplay emits the CollisionDisplay::finished() signal.
     */
    void setDisplayFinished();

    /**
     * @brief Changes the lookahead time to the specified value.
     *
     * Sets the value of LOOKAHEAD_TIMESTEPS by multiplying the desired
     * lookahead time by the current framerate. Called whenever the lookahead
     * input field is changed.
     *
     * @param d the new lookahead time in seconds.
     */
    void setLookaheadTime(double d);

    /**
     * @brief Sets the new target framerate for the display.
     *
     * Changes the value of FRAMERATE to the provided value, and uses the new
     * framerate to recalculate the value of LOOKAHEAD_TIMESTEPS such that the
     * lookahead time stays the same. Note that the framerate will only change
     * in between timesteps, so the effect of this method is not immediate.
     * Called when the framerate slider is dragged.
     *
     * @param n the target framerate in frames per second.
     */
    void setTargetFramerate(int n);

    /**
     * @brief Sets the maximum value for the minimum radius input.
     *
     * Changes the maximum possible value for the minimum Particle radius input
     * to the value of the maximum radius input (i.e. the minimum radius cannot
     * be set above the maximum radius). Called when the maximum radius input
     * changes.
     *
     * @param d the new maximum for the minimum Particle radius.
     */
    void setMinRadiusMax(double d);

    /**
     * @brief Sets the minimum value for the maximum radius input.
     *
     * Changes the minimum possible value for the maximum Particle radius input
     * to the value of the minimum radius input (i.e. the maximum radius cannot
     * be set below the minimum radius). Called when the maximum radius input
     * changes.
     *
     * @param d the new minimum for the maximum Particle radius.
     */
    void setMaxRadiusMin(double d);

    /**
     * @brief Sets a limit on the number of timesteps the Algorithm will run.
     *
     * Sets the value of TIMESTEP_LIMIT so that the Algorithm and display will
     * quit running on that timestep. In truth, an Algorithm will keep running
     * until TIMESTEP_LIMIT + LOOKAHEAD_TIMESTEPS so that it can continue
     * warning about Collision%s that don't actually happen during the timestep
     * limit.
     *
     * @param n the new value for the timestep limit.
     */
    void setTimestepLimit(int n);

    /**
     * @brief Enables or disables the timestep limit input field.
     *
     * Based on the value of the input boolean, enables or disables the
     * timestep input field and sets the value of TIMESTEP_LIMIT to the value
     * of the field (if true) or -1 (if false).
     *
     * @param b the eneabled state of the timestep limit field.
     */
    void setTimestepLimitEnabled(bool b);

    /**
     * @brief Sets the variable representing the simulation dimensionality.
     *
     * Sets a variable which determines if the simulation is in 2D, 3D, or orbital and
     * switches the corresponding display to be visible. Changes the maximum
     * value of the framerate slider (MAX_FRAMERATE in 2D and 60 in 3D and orbital).
     * Enables (3D/orbital) or disables (2D) the Reset Camera button.
     *
     * @param b a int represeting whether simulation set to 2D, 3D, or orbital.
     */
    void setSpace(int b);

signals:
    /**
     * @brief A signal emitted to begin the Algorithm and display.
     */
    void begin();
};

#endif // COLLISIONSYSTEM_H
