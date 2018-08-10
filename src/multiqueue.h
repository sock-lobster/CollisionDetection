#ifndef MULTIQUEUE_H
#define MULTIQUEUE_H
#include "particle.h"
#include "node.h"
#include <mutex>
#include <condition_variable>

/**
 * @brief Custom queue for managing Position%s of Particle%s.
 *
 * MultiQueue manages Node%s which carry Position information for a single
 * timestep. The underlying data structure is a doubly linked list. In addition
 * to functioning as a normal queue, Node%s in the middle of the MultiQueue may
 * be accessed and dequeued from the queue. This feature is used to pop Node%s
 * of an arbitrary timestep for a given Particle which is required by the
 * CollisionDisplay. MultiQueue also includes its own lock for synchronization.
 */
class MultiQueue
{   
public:
    /**
     * @brief Constructs a new MultiQueue.
     *
     * Constructs a new MultiQueue, initializing the head and tail pointers
     * to NULL and the size to 0.
     */
    MultiQueue();
    ~MultiQueue();

    /**
     * @brief Returns the size of the queue.
     * @return The size of the queue.
     */
    int getSize();

    /**
     * @brief Returns the mutex for the queue.
     * @return The mutex associated with the queue.
     */
    std::mutex* getPositionMutex();

    /**
     * @brief Returns the condition variable used to wait when the queue is
     * full.
     *
     * @return The condition variable for waiting when the queue is full.
     */
    std::condition_variable* getQueueNotFull();

    /**
     * @brief Returns a pointer to the head of the queue.
     * @return A pointer to the head of the queue.
     */
    Node* getHead();

    /**
    * @brief Returns a pointer to the tail of the queue.
    * @return A pointer to the tail of the queue.
    */
    Node* getTail();

    /**
     * @brief Returns a pointer to the nth Node for the given Particle.
     *
     * Either returns a pointer to the nth Node associated with the given
     * Particle, the last Node if there aren't n, or NULL if there are none.
     *
     * @param n the number of the Node to return.
     * @param p a pointer to the Particle.
     * @return A pointer to the nth Node, the last Node if there aren't n, or
     * NULL if there are none.
     */
    Node* getNthInstance(int n, Particle* p);

    /**
     * @brief Returns a pointer to the nth Node for the given Particle and
     * removes the Node from the queue.
     *
     * Functions identically to getNthInstance(), but also removes the Node
     * from the queue.
     *
     * @param n the number of the Node to return.
     * @param p a pointer to the Particle.
     * @return A pointer to the nth Node or the last Node if there aren't n.
     */
    Node* popNthInstance(int n, Particle* p);

    /**
     * @brief Returns a pointer to the first Node for the given Particle and
     * removes the Node from the queue.
     *
     * Simply calls popNthInstance() with n = 0.
     *
     * @param p a pointer to the Particle.
     * @return A pointer to the first Node.
     */
    Node* popFirstInstance(Particle* p);

    /**
     * @brief Returns a pointer to the Node for the given Particle and timestep.
     *
     * Returns either a pointer to the Node associated with the given Particle,
     * the last Node for that Particle, or NULL if there are no Nodes for that
     * Particle.
     *
     * @param n the requested timestep.
     * @param p a pointer to the Particle.
     * @return A pointer to the Node as described above or NULL.
     */
    Node* getTimestepN(int n, Particle* p);

    /**
     * @brief Returns a pointer to the Node for the given Particle and timestep
     * and removes the Node.
     *
     * Effectively the same as getTimestepN(), but also removes the returned
     * Node and all earlier Nodes associated with the Particle from the queue.
     *
     * @param n the requested timestep.
     * @param p a pointer to the Particle.
     * @return A pointer to the Node as described above or NULL.
     */
    Node* popTimestepN(int n, Particle* p);

    /**
     * @brief Returns a pointer to the first Node in the queue and removes the
     * Node from the queue.
     * @return a pointer to the first Node.
     */
    Node* pop();

    /**
     * @brief Push a Node onto the head of the queue.
     */
    void pushHead(Node*);

    /**
     * @brief Enqueue a Node onto the tail of the queue.
     */
    void pushTail(Node*);

private:
    Node* head;
    Node* tail;
    int size;

    std::mutex positionMutex;
    std::condition_variable queueNotFull;
};

#endif // MULTIQUEUE_H
