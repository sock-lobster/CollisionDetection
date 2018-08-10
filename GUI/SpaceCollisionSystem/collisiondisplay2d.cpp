#include "collisiondisplay2d.h"
#include "constants.h"

CollisionDisplay2D::CollisionDisplay2D(QWidget *parent) :
    QGraphicsView(parent), CollisionDisplay()
{
    setBackgroundBrush(QColor(26,26,26));
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,X_DIMENSION_SIZE, Y_DIMENSION_SIZE);
    fitInView(scene->sceneRect());
    setScene(scene);

    timer = new QTimer;
    numCollisions = 0;
}

CollisionDisplay2D::~CollisionDisplay2D() {
    delete scene;
}

void CollisionDisplay2D::start() {
    // reinitialize stuff
    displayTimestep = 0;
    numCollisions = 0;
    drawnParticles.clear();
    particleMap.clear();
    std::queue<int> empty;
    std::swap( collisionTimesteps, empty );

    std::vector<Particle*> particles = qalg->getParticles();
    for (Particle* p : particles) {
        QParticle2D* curDrawnParticle = new QParticle2D(p);
        scene->addItem(curDrawnParticle);
        // add new drawn particle to list of drawnParticles
        drawnParticles.push_back(curDrawnParticle);
        // add particle/drawn particle pair to map
        std::pair<Particle*, QParticle2D*> pair(p, curDrawnParticle);
        particleMap.insert(pair);
    }
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    positionQueue = qalg->getAlgorithm()->getPositionQueue();

    timer->start(1000 / FRAMERATE);
}

void CollisionDisplay2D::stop() {
    timer->stop();
    delete timer;
    timer = new QTimer;

    scene->clear();
    qalg = NULL;

    emit finished();
}

void CollisionDisplay2D::step() {
    if (qalg->getAlgorithm()->getTimestep() / ALG_SPEED_SCALE > displayTimestep)
    {
        displayTimestep++;
        for (QParticle2D* dp : drawnParticles) {
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
    for (QParticle2D* dp : drawnParticles) {
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
        QParticle2D* da = particleMap[a];
        QParticle2D* db = particleMap[b];
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

QTimer* CollisionDisplay2D::getTimer() {
    return timer;
}

QGraphicsScene* CollisionDisplay2D::getScene() {
    return scene;
}

void CollisionDisplay2D::setQAlgorithm(QAlgorithm* qalgorithm) {
    qalg = qalgorithm;
}
