#ifndef COLLISIONDISPLAY2D_H
#define COLLISIONDISPLAY2D_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <unordered_map>
#include "collisiondisplay.h"
#include "qalgorithm.h"
#include "qparticle2d.h"

/**
 * @brief The view containing 2D visualization.
 *
 *  CollisionDisplay2D creates and displays a 2D visualization of a simulation.
 *  It keeps track of a QParticle2D for each Particle in the environment and it
 *  updates their drawing on the scene at every timestep.
 *
 *  It inherits from QGraphicsView and CollisionDisplay.
 */
class CollisionDisplay2D : public QGraphicsView, public CollisionDisplay
{
    Q_OBJECT
public:

    /**
     * @brief Constructs a CollisionDisplay2D.
     *
     * Calls CollisionDisplay constructor and QGraphicsView constructor.
     * Initializes the QGraphicsScene scene and sets its dimensions.
     * Initializes the QTimer timer used to continually update the graphics.
     *
     * @param parent the parent window if one exists.
     */
    explicit CollisionDisplay2D(QWidget *parent = 0);
    ~CollisionDisplay2D();

    /**
     * @brief Gets the scene that is declared in CollisionDisplay2D.
     * @return Pointer to a QGraphicsScene scene.
     */
    QGraphicsScene* getScene();

    /**
     * @brief Gets the timer declared in CollisionDisplay.
     * @return Pointer to a QTimer timer.
     */
    QTimer* getTimer();

    /**
     * @brief Sets the Algorithm declared in CollisionDisplay.
     * @param Pointer to the QAlgorithm being used.
     */
    void setQAlgorithm(QAlgorithm *qalgorithm);

signals:
    /**
     * @brief Signals that the display has finished. Usually because the user
     * clicked "stop", for example.
     *
     * This signal allows CollisionSystem to start a new simulation or exit safely.
     */
    void finished();

public slots:

    /**
     * @brief Starts the simulation.
     *
     * Several values are reinitialized to values they should be at the beginning of a simulation.
     * A QParticle2D qp is created and mapped to every Particle in the environment.
     * Every QParticle2D is then added to the scene that will be displayed.
     *
     * Finally, the timer is started.
     */
    void start();

    /**
     * @brief Stops the timer and reinitializes the QTimer timer, QGraphicsScene scene, QAlgorithm qalg, and int numCollisions.
     * Emits the signal finished.
     */
    void stop();

    /**
     * @brief Performs one step of the display.
     *
     * Increments the display timestep and checks it against the TIMESTEP_LIMIT
     * to see if the display should continue running. If so, attempts to update
     * the Position of each QParticle2D and mark them as colliding if they will
     * collide within the specified window. Finally, updates the timer interval
     * to allow the framerate to be changed in real time.
     */
    void step();

private:
    QGraphicsScene* scene;

    std::vector<QParticle2D*> drawnParticles;
    std::unordered_map<Particle*, QParticle2D*> particleMap;
};

#endif // COLLISIONDISPLAY2D_H
