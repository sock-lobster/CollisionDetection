#include "node.h"

Node::Node(Particle* part, Position* pos) {
    particle = part;
    position = pos;

    next = NULL;
    previous = NULL;
    nextForParticle = NULL;
    previousForParticle = NULL;
}

Node::~Node()
{
    delete position;
}

Node* Node::getNext() {
    return next;
}

Node* Node::getNextForParticle() {
    return nextForParticle;
}

Node* Node::getPrevious() {
    return previous;
}

Node* Node::getPreviousForParticle() {
    return previousForParticle;
}

void Node::setNext(Node* n) {
    next = n;
}

void Node::setNextForParticle(Node* n) {
    nextForParticle = n;
}

void Node::setPrevious(Node* n) {
    previous = n;
}

void Node::setPreviousForParticle(Node* n) {
    previousForParticle = n;
}

Particle* Node::getParticle() {
    return particle;
}

Position* Node::getPosition() {
    return position;
}
