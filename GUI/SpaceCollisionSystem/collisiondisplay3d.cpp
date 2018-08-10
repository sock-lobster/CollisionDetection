#include <QGLBuilder>
#include <QGLSphere>
#include <QGLCube>
#include "collisiondisplay3d.h"
#include "constants.h"

CollisionDisplay3D::CollisionDisplay3D(QWindow *parent) :
    QGLView(parent), CollisionDisplay()
{
    buildBoundaryCube();
}

CollisionDisplay3D::~CollisionDisplay3D() {}

void CollisionDisplay3D::buildBoundaryCube() {
    double cubeSide = X_DIMENSION_SIZE;

    boundaryCube.append(0, 0, 0);
    boundaryCube.append(cubeSide, 0, 0);

    boundaryCube.append(0, 0, 0);
    boundaryCube.append(0, cubeSide, 0);

    boundaryCube.append(0, 0, 0);
    boundaryCube.append(0, 0, cubeSide);

    boundaryCube.append(cubeSide, 0, 0);
    boundaryCube.append(cubeSide, cubeSide, 0);

    boundaryCube.append(cubeSide, 0, 0);
    boundaryCube.append(cubeSide, 0, cubeSide);

    boundaryCube.append(cubeSide, cubeSide, 0);
    boundaryCube.append(0, cubeSide, 0);

    boundaryCube.append(cubeSide, 0, cubeSide);
    boundaryCube.append(0, 0, cubeSide);

    boundaryCube.append(0, cubeSide, cubeSide);
    boundaryCube.append(0, 0, cubeSide);

    boundaryCube.append(0, cubeSide, cubeSide);
    boundaryCube.append(0, cubeSide, 0);

    boundaryCube.append(cubeSide, cubeSide, cubeSide);
    boundaryCube.append(0, cubeSide, cubeSide);

    boundaryCube.append(cubeSide, 0, cubeSide);
    boundaryCube.append(cubeSide, cubeSide, cubeSide);

    boundaryCube.append(cubeSide, cubeSide, 0);
    boundaryCube.append(cubeSide, cubeSide, cubeSide);
}

void CollisionDisplay3D::start() {
    // reinitialize stuff
    displayTimestep = 0;
    numCollisions = 0;
    std::queue<int> empty;
    std::swap(collisionTimesteps, empty);

    if (qalg->getAlgorithm()->getPhysics()->isOrbital()) {
        Particle* centerOfGravity = qalg->getCenterOfGravity();
        QParticle3D* cog = new QParticle3D(centerOfGravity);
        cog->setPosition(centerOfGravity->getCurPos());
        drawnParticles.push_back(cog);
        std::pair<Particle*, QParticle3D*> cogPair(centerOfGravity, cog);
        particleMap.insert(cogPair);
    }

    std::vector<Particle*> particles = qalg->getParticles();
    for (Particle* p : particles)
    {
        QParticle3D* curDrawnParticle = new QParticle3D(p);
        // add new drawn particle to list of drawnParticles
        drawnParticles.push_back(curDrawnParticle);
        // add particle/drawn particle pair to map
        std::pair<Particle*, QParticle3D*> pair(p, curDrawnParticle);
        particleMap.insert(pair);
    }
    positionQueue = qalg->getAlgorithm()->getPositionQueue();

    timer->start(1000 / FRAMERATE);
}

void CollisionDisplay3D::stop() {
    timer->stop();
    delete timer;
    timer = new QTimer;

    drawnParticles.clear();
    particleMap.clear();
    update();

    qalg = NULL;

    emit finished();
}

void CollisionDisplay3D::step() {
    // prevent the display timestep from exceeding the algorithm timestep
    if (qalg->getAlgorithm()->getTimestep()/ALG_SPEED_SCALE > displayTimestep)
    {
        displayTimestep++;
        for (QParticle3D* dp : drawnParticles) {
            dp->incrementTimestep();
        }
    }

    // check if we should keep running
    if (TIMESTEP_LIMIT != -1 && displayTimestep >= TIMESTEP_LIMIT / ALG_SPEED_SCALE) {
        stop();
        return;
    }

    // update positions
    std::mutex* posMutex = positionQueue->getPositionMutex();
    for (QParticle3D* dp : drawnParticles) {
        std::unique_lock<std::mutex> posLock(*posMutex);

        Node* node = positionQueue->popTimestepN(displayTimestep * ALG_SPEED_SCALE, dp->getParticle());

        if (node != NULL) {
            Position* newPos = node->getPosition();
            dp->setPosition(newPos);
            delete node;
        }

        positionQueue->getQueueNotFull()->notify_all();
    }

    // update collision information
    std::mutex* collisionMutex = qalg->getAlgorithm()->getCollisionMutex();
    std::queue<Collision*>* collisionQueue = qalg->getAlgorithm()->getCollisions();

    while (!collisionQueue->empty() && collisionQueue->front()->getCollisionTimestep() < (displayTimestep + LOOKAHEAD_TIMESTEPS) * ALG_SPEED_SCALE) {
        collisionMutex->lock();
        Collision* newCollision = collisionQueue->front();
        collisionQueue->pop();
        collisionMutex->unlock();

        collisionTimesteps.push(newCollision->getCollisionTimestep());
        Particle* a = newCollision->getColliderA();
        Particle* b = newCollision->getColliderB();
        QParticle3D* da = particleMap[a];
        QParticle3D* db = particleMap[b];
        da->setCollisionTimestep(newCollision->getCollisionTimestep() / ALG_SPEED_SCALE);
        db->setCollisionTimestep(newCollision->getCollisionTimestep() / ALG_SPEED_SCALE);

        delete newCollision;
    }

    while (!collisionTimesteps.empty() && collisionTimesteps.front() < displayTimestep * ALG_SPEED_SCALE) {
        collisionTimesteps.pop();
        numCollisions++;
    }

    timer->setInterval(1000 / FRAMERATE);
}

void CollisionDisplay3D::paintGL(QGLPainter* painter)
{
    // background color
    painter->setClearColor(QColor(26,26,26));

    if (qalg != NULL && !qalg->getAlgorithm()->getPhysics()->isOrbital())
    {
        painter->clearAttributes();
        painter->setStandardEffect(QGL::FlatColor);
        painter->setVertexAttribute(QGL::Position , boundaryCube);
        painter->setColor(QColor(255,255,255));
        painter->draw(QGL::Lines, boundaryCube.size());
    }

    for (QParticle3D* dp : drawnParticles)
    {
        dp->draw(painter);
    }
}
