#ifndef QPARTICLE3D_H
#define QPARTICLE3D_H

#include <valarray>
#include <QGLSceneNode>
#include <QGLPainter>
#include <QGLTexture2D>
#include <QGLMaterial>
#include "particle.h"

/**
 * @brief 3D graphical wrapper for Particle.
 *
 * QParticle3D provides a graphical wrapper for Particle for 3D visualization.
 * Each QParticle3D is associated with a Particle and allows that Particle to
 * be drawn into the CollisionDisplay3D.
 */
class QParticle3D
{
public:
    /**
     * @brief Constructs a QParticle3D associated with the given Particle.
     *
     * Initializes the actual sphere for visualization and sets the initial
     * pPosition of the sphere.
     *
     * @param p the Particle with which to associate.
     */
    QParticle3D(Particle *p);

    /**
     * @brief Returns the Particle associated with this QParticle3D.
     * @return The associated Particle.
     */
    Particle* getParticle();

    /**
     * @brief Sets the current Position of the QParticle3D.
     * @param newpos the new Position for the QParticle3D.
     */
    void setPosition(Position *newpos);

    /**
     * @brief Sets the timestep for the a Collision involving the Particle.
     *
     * Once marked for a Collision by this method, the QParticle3D will be
     * drawn in red until the Collision happens.
     *
     * @param t the timestep of the Collision.
     */
    void setCollisionTimestep(int t);

    /**
     * @brief Increments the internal timestep counter.
     *
     * Increments the internal timestep counter to keep it in sync with
     * CollisionDisplay's timestep counter.
     */
    void incrementTimestep();

    /**
     * @brief Draw the QParticle3D to the display.
     *
     * Draws the QParticle3D depending on a few different factors. If the
     * QParticle3D is marked for Collision, it will be painted red. Otherwise,
     * it will be painted green. If the Particle represents the center of mass,
     * it will instead be textured with an image of the earth.
     *
     * @param painter the object used to draw the Particle to a QGLView.
     */
    void draw(QGLPainter* painter);

private:
    Particle* particle;
    QGLSceneNode* sphere;
    std::valarray<double> pos;
    QColor color;
    QGLTexture2D earthSkin;
    int timestep;
    int collisionTimestep;
};

#endif // QPARTICLE3D_H
