#include "qparticle3d.h"
#include "constants.h"
#include <QGLBuilder>
#include <QGLSphere>
#include <QGLCube>

QParticle3D::QParticle3D(Particle* p)
{
    particle = p;
    timestep = 0;
    collisionTimestep = -1;

    pos.resize(3, 0);
    if (getParticle()->getID() == -1)
    {
        earthSkin.setImage(QImage(QLatin1String(":resources/world.png")));
    }
    else
    {
        color = QColor(0, 200, 0);
    }

    QGLBuilder builder;
    builder.newSection(QGL::Faceted);
    builder << QGLSphere(p->getRadius() * 2);
    sphere = builder.finalizedSceneNode();
    setPosition(p->getCurPos());
}

Particle* QParticle3D::getParticle() {
    return particle;
}

void QParticle3D::setPosition(Position* newpos) {
    pos = newpos->getPos();
}

void QParticle3D::setCollisionTimestep(int t) {
    collisionTimestep = t;
}

void QParticle3D::incrementTimestep() {
    timestep++;
}

void QParticle3D::draw(QGLPainter* painter) {
    if (particle->getID() != -1)
    {
        painter->setStandardEffect(QGL::LitMaterial);
        if (collisionTimestep >= timestep)
        {
            painter->setFaceColor(QGL::AllFaces, QColor(200, 0, 0));
        }
        else
        {
            painter->setFaceColor(QGL::AllFaces, color);
        }
    }
    else
    {
        painter->setStandardEffect(QGL::LitDecalTexture2D);
        earthSkin.bind();
    }

    painter->modelViewMatrix().translate(pos[X_DIMENSION], pos[Y_DIMENSION], pos[Z_DIMENSION]);
    sphere->draw(painter);

    painter->modelViewMatrix().translate(-pos[X_DIMENSION], -pos[Y_DIMENSION], -pos[Z_DIMENSION]);
}
