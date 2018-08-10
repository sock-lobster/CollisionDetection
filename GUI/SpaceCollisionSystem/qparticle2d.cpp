#include "qparticle2d.h"
#include "constants.h"
#include <QMutex>
#include <iostream>

QParticle2D::QParticle2D(Particle* p)
{
    particle = p;
    radius = p->getRadius() * SIZE_MULTIPLIER_2D;
    diameter = radius*2;
    color = QColor(0, 200, 0);

    timestep = 0;
    collisionTimestep = -1;
    setPosition(p->getCurPos());
}

QParticle2D::~QParticle2D() {}

QRectF QParticle2D::boundingRect() const {
    return QRectF(x(), y(), diameter * 2, diameter * 2);
}

void QParticle2D::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(color);
    painter->setPen(color);
    painter->drawEllipse(x(), y(), diameter * 2, diameter * 2);
}

void QParticle2D::advance(int step) {
    if (step == 1){
        if (collisionTimestep >= timestep) {
            int red = 200;
            color = QColor(red, 0, 0);
        }
        else {
            color = QColor(0, 200, 0);
        }
    }
}

Particle* QParticle2D::getParticle() {
    return particle;
}

void QParticle2D::setPosition(Position* position) {
    std::valarray<double> temp = position->getPos() * SIZE_MULTIPLIER_2D;
    setPos(temp[X_DIMENSION] - radius, temp[Y_DIMENSION] - radius);
}

void QParticle2D::setCollisionTimestep(int t) {
    collisionTimestep = t;
}

void QParticle2D::incrementTimestep() {
    timestep++;
}
