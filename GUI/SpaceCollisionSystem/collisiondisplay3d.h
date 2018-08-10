#ifndef COLLISIONDISPLAY3D_H
#define COLLISIONDISPLAY3D_H
#include <QGLView>
#include <unordered_map>
#include "collisiondisplay.h"
#include "qparticle3d.h"

/**
 * @brief The view containing 3D visualization.
 *
 * CollisionDisplay3D inherits from the Qt3D class QGLView to display a 3D
 * visualization of a simulation. It keeps track of and draws QParticle3D%s
 * for each Particle of the simulation and allows interaction with the view
 * to change the viewing location.
 */
class CollisionDisplay3D : public QGLView, public CollisionDisplay
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a CollisionDisplay3D.
     *
     * Calls CollisionDisplay's constructor to initialize common members and
     * builds the boundary cube representing the space in which the simulation
     * runs.
     *
     * @param parent the parent window, if one exists.
     */
    explicit CollisionDisplay3D(QWindow *parent = 0);
    ~CollisionDisplay3D();
    
signals:
    /**
     * @brief Signals that the display has finished.
     *
     * Signals that the display has finished, so CollisionSystem can start a
     * new simulation or exit safely once QAlgorithm::finished() occurs.
     */
    void finished();
    
public slots:
    /**
     * @brief Reinitializes counters and lists and begins the timer.
     *
     * Reinitializes the timestep and Collision counters and builds the list
     * of QParticle3D%s from an Algorithm's list of Particle%s. Starts the
     * display timer to begin updating the display.
     */
    void start();

    /**
     * @brief Stops the timer and clears lists.
     *
     * Stops the timer so the display no longer updates and clears the list of
     * QParticle3D%s. Emits the finished() signal.
     */
    void stop();

    /**
     * @brief Performs one step of the display.
     *
     * Increments the display timestep and checks it against the TIMESTEP_LIMIT
     * to see if the display should continue running. If so, attempts to update
     * the Position of each QParticle3D and mark them as colliding if they will
     * collide within the specified window. Finally, updates the timer interval
     * to allow the framerate to be changed in real time.
     */
    void step();

private:
    void paintGL(QGLPainter* painter);

    QVector3DArray boundaryCube;
    void buildBoundaryCube();

    std::vector<QParticle3D*> drawnParticles;
    std::unordered_map<Particle*, QParticle3D*> particleMap;
};

#endif // COLLISIONDISPLAY3D_H
