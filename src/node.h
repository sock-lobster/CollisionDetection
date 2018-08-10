#ifndef NODE_H
#define NODE_H
#include "position.h"
#include "particle.h"

class Particle;

/**
 * @brief The Node class contains a Position for a Particle in the MultiQueue.
 *
 * The Node holds a Position for a Particle, and contains pointers to the
 * previous Node and next Node in the MultiQueue, as well as pointers to the
 * prevoius and next Node%s that are instances of the given Particle.
 */
class Node
{
public:
    /**
     * @brief Constructs a Node for the given Particle containing the Position.
     *
     * This sets the appropriate pointers for the next and previous Node%s in
     * the Queue, and for the next and prevoius Node%s related to the given
     * Particle.
     *
     * @param part The Particle this Node and Position relate to.
     * @param pos the Position this Node is storing
     */
    Node(Particle* part, Position* pos);
    ~Node();

    /**
     * @brief Retruns the next Node in the MultiQueue.
     * @return The next Node in the MultiQueue or NULL.
     */
    Node* getNext();

    /**
     * @brief Returns the next Node in the MultiQueue for this Particle.
     * @return The next Node for this Particle in MultiQueue or NULL.
     */
    Node* getNextForParticle();

    /**
     * @brief Returns the previous Node in the MultiQueue.
     * @return The previous Node in the MultiQueue or NULL.
     */
    Node* getPrevious();

    /**
     * @brief Returns the prevoius Node in the MultiQueue for this Particle.
     * @return The prevous Node for this Particle in the MultiQueue or NULL.
     */
    Node* getPreviousForParticle();

    /**
     * @brief Sets this Node's next pointer to the given Node.
     * @param n the node we are setting to be next after this Node.
     */
    void setNext(Node* n);

    /**
     * @brief Sets this Node's pointer to the next Node with the same Particle.
     * @param n the Node to set to be next after this Node for the Particle.
     */
    void setNextForParticle(Node* n);

    /**
     * @brief Sets this Node's prevoius pointer to the given Node.
     * @param n the Node we are setting to be before this Node.
     */
    void setPrevious(Node* n);

    /**
     * @brief Sets this Node's pointer to the prevoius Node for this Particle.
     * @param n the Node to set to be before this Node for the Particle.
     */
    void setPreviousForParticle(Node* n);

    /**
     * @brief Returns the Particle associated with the Position in this Node.
     * @return The Particle associated with this Node.
     */
    Particle* getParticle();

    /**
     * @brief Returns the Position this Node is holding.
     * @return The Position this Node stores.
     */
    Position* getPosition();

private:
    Node* next;
    Node* previous;
    Node* nextForParticle;
    Node* previousForParticle;

    Particle* particle;
    Position* position;
};

#endif // NODE_H
