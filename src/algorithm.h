#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "physics2d.h"
#include "multiqueue.h"
#include <mutex>

/**
 * @brief The Algorithm class is a parent to all the specific Algorithm%s we
 * test.
 *
 * Algorithm contains most of the data and methods the various Algorithm%s that
 * we use to detect collisions have in common. It also allows the CollisionSystem
 * to ask for information from Algorithm%s without having to know which type of
 * Algorithm it is running.
 */
class Algorithm
{
public:
    /**
     * @brief Constructs a 2 dimensional Algorithm.
     *
     * Initializes a Physics2D, a MultiQueue for Particle's Position%s, sets
     * running to false, the timestep to 0, the x and y size of the window,
     * sets the z size to 0, and sets the system kinetic energy to 0.  Other
     * things that must be initialized are handled by Algorithm's children.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     */
    Algorithm(double xIn, double yIn);

    /**
     * @brief Constructs a 3 dimensional Algorithm.
     *
     * Initializes a Physics3D, a MultiQueue for Particle's Position%s, sets
     * running to false, the timestep to 0, the x, y, and z size of the window,
     * and sets the system kinetic energy to 0.  Other things that must be
     * initialized are handled by Algorithm's children.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     */
    Algorithm(double xIn, double yIn, double zIn);

    /**
     * @brief Constructs a 3 dimensional Algorithm with orbital Physics.
     *
     * Initializes an OrbitalPhysics, a MultiQueue for Particle's Position%s,
     * sets running to false, the timestep to 0, the x, y, and z size of the
     * window, and sets the system kinetic energy to 0.  Other things that must
     * be initialized are handled by Algorithm's children.
     *
     * @param xIn the size of the x dimension of the space.
     * @param yIn the size of the y dimension of the space.
     * @param zIn the size of the z dimension of the space.
     * @param centerMass the mass of the Particle responsible for gravitation.
     */
    Algorithm(double xIn, double yIn, double zIn, double centerMass);

    virtual ~Algorithm();

    /**
     * @brief Returns the vector of Particle%s that the algorithm is tracking.
     * @return The vector of Particle%s the algorithm is tracking.
     */
    std::vector<Particle*> getParticles();

    /**
     * @brief Returns the queue of Collision%s that we haven't entered the
     * warning window yet.
     *
     * When the Algorithm detects a Collision, it pushes it onto this queue.
     * The Collision is removed from the queue when the timestep of the
     * Collision is within the warning window of the CollisionDisplay.  More
     * directly, a Collision leaves this queue when its particles turn red.
     *
     * @return The queue of Collision%s we have detected, but haven't been
     * warned about.
     */
    std::queue<Collision*>* getCollisions();

    /**
     * @brief Returns the MultiQueue that contains upcoming Position%s for
     * each Particle.
     * @return The multiQueue of upcoming Position%s.
     */
    MultiQueue* getPositionQueue();

    /**
     * @brief Returns the Mutex locking the Collision queue.
     * @return The Mutex locking the Collision queue.
     */
    std::mutex* getCollisionMutex();

    /**
     * @brief Returns the massive Particle in the center responsible for
     * gravitation.
     * @return The Particle responsible for gravity and orbits.
     */
    Particle* getCenterOfGravity();

    /**
     * @brief Returns the number of Particle%s the Algorithm is tracking.
     * @return The number of Particle%s moving in the space.
     */
    int getNumParts();

    /**
     * @brief Returns the timestep the Algorithm is currently calculating.
     *
     * The timestep the Algorithm is on is proportional to the timestep the
     * CollisionDisplay is on.  That is because the Algorithm updates Particle
     * Position%s ALG_SPEED_SCALE (usually 50) times for every time the
     * CollisionDisplay refreshes so that gravity calculating can be more
     * precise while still having the Particle%s move at a reasonable rate.
     *
     * @return The Algorithm timestep.
     */
    int getTimestep();

    /**
     * @brief Returns the system kinetic energy which is updated each timestep.
     * @return The system kinetic energy.
     */
    double getSystemKE();

    /**
     * @brief Returns system potential energy which is updated each timestep.
     * @return The system potential energy.
     */
    double getSystemPE();

    /**
     * @brief Gets the number of checks.
     *
     *  Gets the number of times the Algorithm checks for a Collision.
     *
     * @return The number of checks for Collision.
     */
    int getChecks();

    /**
     * @brief Returns the Physic%s object working for this Algorithm.
     * @return The Physic%s associated with the Algorithm.
     */
     Physics* getPhysics();

    /**
     * @brief Sets the number of Particle%s the Algorithm is tracking.
     * @param numParticles the number of Particle%s the Algorithm will have.
     */
    void setNumParts(int numParticles);

    /**
     * @brief Sets the number of Particle%s, then generates that many to track.
     *
     * First this sets the number of Particle%s the Algorithm will be tracking.
     * Then it randomly generates that many 2D Particle%s within the space,
     * ensuring that no two Particle%s are generated overlapping each other.
     * A 2D Particle is a particle with 0 for the z coordinate and velocity.
     *
     * @param numParticles the number of Particle%s the Algorithm will have.
     */
    void createParticles2D(int numParticles);

    /**
     * @brief Sets the number of Particle%s, then generates that many to track.
     *
     * First this sets the number of Particle%s the Algorithm will be tracking.
     * Then it randomly generates that many 3D Particle%s within the space,
     * ensuring that no two Particle%s are generated overlapping each other.
     *
     * @param numParticles the number of Particle%s the Algorithm will have.
     */
    void createParticles3D(int numParticles);

    /**
     * @brief Sets the number of Particle%s, then generates that many to track.
     *
     * First this sets the number of Particle%s the Algorithm will be tracking.
     * Then it randomly generates that many orbital Particle%s within the space,
     * ensuring that no two Particle%s are generated overlapping each other.
     * The Particle%s are generated with KE + PE = 0 and with directions
     * orthogonal to their position relative to the center of gravity, so they
     * will have (close to) circular orbits around the center of gravity.
     *
     * @param numParticles the number of Particle%s the Algorithm will have.
     */
    void createParticlesOrbit(int numParticles);

    /**
     * @brief Pushes Collision%s from the most recent timestep onto the
     * queue of upcoming Collision%s.
     *
     * For each Collision in the list of new Collision%s found in the most
     * recent timestep, this aquires the lock on the Collision queue, pushes a
     * Collision, then releases the lock.
     *
     * @param collisions the vector of Collision%s detected in the most recent
     * timestep.
     */
    void enqueueCollisions(std::vector<Collision*> collisions);

    /**
     * @brief Calculates the next timestep.
     *
     * Calculates the next timestep which entails moving all the Particle%s an
     * appropriate amount, seeing if any Collision%s have occured, and if so,
     * resolving them.
     * This is a public method that calls the private
     * Algorithm::calculateNextTimestep() and is quite possibly bad coding
     * practice.
     */
    void runOneTimestep();

    /**
     * @brief Continuously does all the calculations to go forward through
     * timesteps as fast as it can.
     *
     * While the running boolean is true, the timecounter gets incremented,
     * then calculateNextTimestep() is called updating Particle Position%s
     * and if necessary resolving Collision%s.
     */
    void run();

    /**
     * @brief Tells the Algorithm to cease its calculationons and makes it stop
     * calculating the next timesteps.
     *
     * This stops the Algorithm from calculating future timesteps, and releases
     * the Algorithm's lock on the MultiQueue.
     */
    void stop();

    /**
     * @brief Calculates the next timestep and sets running to true.
     *
     * Calculates the next timestep which entails moving all the particles an
     * appropriate amount, seeing if any Collision%s have occured, and if so,
     * resolving them.
     * This is a public method that calls the private calculateNextTimestep()
     * and is quite possibly bad coding practice.
     */
    void stepTime();

protected:
    std::vector<Particle*> particles;

    std::queue<Collision*> collisionQueue;
    std::mutex collisionMutex;

    MultiQueue* positionQueue;

    Physics* physics;
    bool running;
    int numParts;
    int timestep;
    double xWinSize;
    double yWinSize;
    double zWinSize;
    double systemKE;
    double systemPE;
    int checks;

    Particle* generateParticleIn2D(int i);
    Particle* generateParticleIn3D(int i);
    Particle* generateParticleInOrbit(int i);
    bool checkForOverlap(Particle* p);

    virtual void calculateNextTimestep() = 0;

    void enqueuePositions();

    void updateSystemKE();
    void updateSystemPE();
    void rescaleVelocities();

    void checkTimestep();
};

#endif // ALGORITHM_H
