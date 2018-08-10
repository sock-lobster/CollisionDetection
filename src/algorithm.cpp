#include "algorithm.h"
#include "physics2d.h"
#include "physics3d.h"
#include "orbitalphysics.h"
#include "constants.h"

Algorithm::Algorithm(double xIn, double yIn)
{
    physics = new Physics2D(xIn, yIn);
    positionQueue = new MultiQueue();
    running = false;
    timestep = 0;
    xWinSize = xIn;
    yWinSize = yIn;
    zWinSize = 0.0;
    systemKE = 0.0;
    checks = 0;
}

Algorithm::Algorithm(double xIn, double yIn, double zIn)
{
    physics = new Physics3D(xIn, yIn, zIn);
    positionQueue = new MultiQueue();
    running = false;
    timestep = 0;
    xWinSize = xIn;
    yWinSize = yIn;
    zWinSize = zIn;
    systemKE = 0.0;
    checks = 0;
}

Algorithm::Algorithm(double xIn, double yIn, double zIn, double centerMass)
{
    physics = new OrbitalPhysics(xIn, yIn, zIn, centerMass);
    positionQueue = new MultiQueue();
    running = false;
    timestep = 0;
    xWinSize = xIn;
    yWinSize = yIn;
    zWinSize = zIn;
    systemKE = 0.0;
    checks = 0;
}

Algorithm::~Algorithm()
{
    delete physics;
    delete positionQueue;
    while(!collisionQueue.empty())
    {
        Collision* c = collisionQueue.front();
        collisionQueue.pop();
        delete c;
    }
    while(!particles.empty())
    {
        Particle* p = particles.back();
        particles.pop_back();
        delete p;
    }
}

Particle* Algorithm::getCenterOfGravity()
{
    return physics->getCenterOfGravity();
}

Particle* Algorithm::generateParticleIn2D(int i)
{
    double radius = physics->randDouble(MIN_PARTICLE_RADIUS, MAX_PARTICLE_RADIUS);

    double mass = pow(radius, 2);

    double xPos = physics->randDouble(0 + radius, X_DIMENSION_SIZE - radius);
    double yPos = physics->randDouble(0 + radius, Y_DIMENSION_SIZE - radius);
    double zPos = 0.0;

    std::valarray<double> vel(3);
    vel[0] = physics->randDouble(MIN_VELOCITY, MAX_VELOCITY) * pow(-1, physics->randBit()) * VELOCITY_MULTIPLIER;
    vel[1] = physics->randDouble(MIN_VELOCITY, MAX_VELOCITY) * pow(-1, physics->randBit()) * VELOCITY_MULTIPLIER;
    vel[2] = 0.0;

    Position* pos = new Position(xPos, yPos, zPos, timestep);
    Particle* p = new Particle(i, pos, vel, radius, mass);
    return p;
}

Particle* Algorithm::generateParticleIn3D(int i)
{
    double radius = physics->randDouble(MIN_PARTICLE_RADIUS, MAX_PARTICLE_RADIUS);

    double mass = pow(radius, 2);

    double xPos = physics->randDouble(0 + radius, X_DIMENSION_SIZE - radius);
    double yPos = physics->randDouble(0 + radius, Y_DIMENSION_SIZE - radius);
    double zPos = physics->randDouble(0 + radius, Z_DIMENSION_SIZE - radius);

    std::valarray<double> vel(3);
    vel[0] = physics->randDouble(MIN_VELOCITY, MAX_VELOCITY) * pow(-1, physics->randBit()) * VELOCITY_MULTIPLIER;
    vel[1] = physics->randDouble(MIN_VELOCITY, MAX_VELOCITY) * pow(-1, physics->randBit()) * VELOCITY_MULTIPLIER;
    vel[2] = physics->randDouble(MIN_VELOCITY, MAX_VELOCITY) * pow(-1, physics->randBit()) * VELOCITY_MULTIPLIER;

    Position* pos = new Position(xPos, yPos, zPos, timestep);
    Particle* p = new Particle(i, pos, vel, radius, mass);
    return p;
}

Particle* Algorithm::generateParticleInOrbit(int i)
{
    std::valarray<double> uv (3);
    uv[0] = physics->randDouble(-1, 1);
    uv[1] = physics->randDouble(-1, 1);
    uv[2] = physics->randDouble(-1, 1);

    uv = uv / physics->getMagnitude(uv) * (physics->randGamma(1, 2) + physics->getCenterOfGravity()->getRadius()) + X_DIMENSION_SIZE/2;

    Position* pos = new Position(uv[0], uv[1], uv[2], timestep);

    // vector from centerOfGravity to particle
    std::valarray<double> orbitVector = pos->getPos() - physics->getCenterOfGravity()->getCurPos()->getPos();

    // create vector orthogonal to orbitVector
    std::valarray<double> velocity (3);
    velocity[0] = physics->randDouble(-1,1);
    velocity[1] = physics->randDouble(-1,1);
    velocity[2] = -(orbitVector[0] * velocity[0] + orbitVector[1] * velocity[1]) / orbitVector[2];

    velocity = velocity / physics->getMagnitude(velocity);
    velocity = velocity * pow((GRAVITY * physics->getCenterOfGravity()->getMass()) / physics->getMagnitude(orbitVector), 0.5);

    double radius = physics->randDouble(MIN_PARTICLE_RADIUS, MAX_PARTICLE_RADIUS);

    double mass = pow(radius, 2);

    Particle* p = new Particle(i, pos, velocity, radius, mass);

    return p;
}

bool Algorithm::checkForOverlap(Particle* p)
{
    if (physics->isOrbital())
    {
        if (physics->particlesOverlap(p, physics->getCenterOfGravity()))
        {
            delete p;
            return true;
        }
    }

    for (Particle* o : particles)
    {
        if (physics->particlesOverlap(p, o))
        {
            delete p;
            return true;
        }
    }
    return false;
}

void Algorithm::createParticles2D(int numParticles)
{
    setNumParts(numParticles);

    for (int i = 0; i<numParticles; i++)

    {
        Particle* p = generateParticleIn2D(i);
        while (checkForOverlap(p))
        {
            p = generateParticleIn2D(i);
        }
        particles.push_back(p);
    }
}

void Algorithm::createParticles3D(int numParticles)
{
    setNumParts(numParticles);

    for (int i = 0; i<numParticles; i++)

    {
        Particle* p = generateParticleIn3D(i);
        while (checkForOverlap(p))
        {
            p = generateParticleIn3D(i);
        }
        particles.push_back(p);
    }
}

void Algorithm::createParticlesOrbit(int numParticles)
{
    setNumParts(numParticles);

    for (int i = 0; i < numParticles; i++)
    {
        Particle* p = generateParticleInOrbit(i);
        while (checkForOverlap(p))
        {
            p = generateParticleInOrbit(i);
        }
        physics->initializeAcceleration(p);
        particles.push_back(p);
    }
}

std::vector<Particle*> Algorithm::getParticles() {
    return particles;
}

std::queue<Collision*>* Algorithm::getCollisions() {
    return &collisionQueue;
}

int Algorithm::getNumParts()
{
    return numParts;
}

int Algorithm::getTimestep()
{
    return timestep;
}

MultiQueue* Algorithm::getPositionQueue()
{
    return positionQueue;
}

std::mutex* Algorithm::getCollisionMutex() {
    return &collisionMutex;
}

double Algorithm::getSystemKE() {
    return systemKE;
}

double Algorithm::getSystemPE() {
    return systemPE;
}

int Algorithm::getChecks() {
    return checks;
}

Physics* Algorithm::getPhysics() {
    return physics;
}

void Algorithm::setNumParts(int numParticles)
{
    numParts = numParticles;
}

void Algorithm::enqueuePositions()
{
    if (timestep % ALG_SPEED_SCALE != 0)
    {
        return;
    }
    // enqueue all new positions
    for (Particle* p : particles)
    {
        if (!running) {
            break;
        }

        Node* newNode = new Node(p, new Position(p->getCurPos()));
        std::mutex* posMutex = positionQueue->getPositionMutex();
        std::unique_lock<std::mutex> posLock(*posMutex);

        if (positionQueue->getSize() >= MAX_QUEUE_SIZE) {
            positionQueue->getQueueNotFull()->wait(posLock);
        }

        positionQueue->pushTail(newNode);
    }
}

void Algorithm::enqueueCollisions(std::vector<Collision*> collisions)
{
    for (std::vector<Collision*>::iterator cur = collisions.begin(); cur != collisions.end(); ++cur) {
        collisionMutex.lock();

        collisionQueue.push(*cur);

        collisionMutex.unlock();
    }
}

void Algorithm::runOneTimestep()
{
    calculateNextTimestep();
}

void Algorithm::run()
{
    int timeCounter = 0;
    running = true;

    while (running)
    {
        timeCounter += 1;
        calculateNextTimestep();
        updateSystemKE();
        updateSystemPE();

        // check if we are passed timestep limit
        checkTimestep();
    }
}

void Algorithm::stop()
{
    running = false;
    for (Particle* p : particles) {
        positionQueue->getQueueNotFull()->notify_all();
    }
}

void Algorithm::updateSystemKE() {
    double newKE = 0;
    for (Particle* p : particles) {
        newKE += physics->calculateKE(p);
    }
    systemKE = newKE;
}

void Algorithm::updateSystemPE() {
    double newPE = 0;
    for (Particle* p : particles) {
        newPE += physics->calculatePE(p);
    }
    systemPE = newPE;
}

void Algorithm::stepTime(){
    running = true;
    calculateNextTimestep();
}

void Algorithm::checkTimestep() {
    if (TIMESTEP_LIMIT != -1 && TIMESTEP_LIMIT + LOOKAHEAD_TIMESTEPS * ALG_SPEED_SCALE <= timestep) {
        running = false;
    }
}
