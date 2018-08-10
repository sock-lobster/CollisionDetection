#ifndef DRAWNPARTICLE_H
#define DRAWNPARTICLE_H
#include <QGraphicsItem>
#include <QPainter>
#include "particle.h"

/**
 * @brief 2D graphical wrapper for Particle.
 *
 * QParticle2D provides a graphical wrapper for Particle for 2D visualization.
 * Each QParticle3D is associated with a Particle and allows that Particle to
 * be drawn into the CollisionDisplay2D.
 */
class QParticle2D : public QGraphicsItem
{
public:
    explicit QParticle2D(Particle *p);
    ~QParticle2D();

    QRectF boundingRect() const;
    void paint(QPainter*painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * @brief Returns the Particle associated with this QParticle2D.
     * @return The associated Particle.
     */
    Particle* getParticle();

    /**
     * @brief Sets the current Position of the QParticle2D.
     * @param position the new Position for the QParticle2D.
     */
    void setPosition(Position* position);

    /**
     * @brief Sets the timestep for the a Collision involving the Particle.
     *
     * Once marked for a Collision by this method, the QParticle2D will be
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

public slots:
    void advance(int step);

private:
    Particle* particle;
    qreal radius;
    qreal diameter;
    QColor color;

    int timestep;
    int collisionTimestep;

};

#endif // DRAWNPARTICLE_H
