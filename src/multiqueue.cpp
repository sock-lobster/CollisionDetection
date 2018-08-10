#include "multiqueue.h"

/* The head is closest to the display
 * The tail is closest to the Algorithm
 * Next means you are going towards the tail
 * Previous means you are going towards the head
 */

MultiQueue::MultiQueue()
{
    head = NULL;
    tail = NULL;
    size = 0;
}

MultiQueue::~MultiQueue() {
    while (size != 0) {
        delete pop();
    }
}

int MultiQueue::getSize() {
    return size;
}

std::mutex* MultiQueue::getPositionMutex() {
    return &positionMutex;
}

std::condition_variable *MultiQueue::getQueueNotFull() {
    return &queueNotFull;
}

Node* MultiQueue::getHead()
{
    return head;
}

Node* MultiQueue::getTail()
{
    return tail;
}

// returns Nth position for a given particle, or the last known position if there aren't N.
// if you only call this when getting position where a collision happens, there
// will always be at least N, so it is fine.  we promise.
Node* MultiQueue::getNthInstance(int n, Particle* p)
{
    Node* pNode = p->getHead();

    for(int i = 0; i < n; i++)
    {
        if (pNode->getNextForParticle() != NULL)
        {
            pNode = pNode->getNextForParticle();
        }
        else
        {
            return pNode;
        }
    }
    return pNode;
}

Node* MultiQueue::popNthInstance(int n, Particle* p)
{
    Node* nNode = getNthInstance(n, p);

    if (nNode->getNextForParticle() != NULL) {
        nNode->getNextForParticle()->setPreviousForParticle(nNode->getPreviousForParticle());
    }
    else {
        p->setTail(nNode->getPreviousForParticle());
    }
    if (nNode->getPreviousForParticle() != NULL) {
        nNode->getPreviousForParticle()->setNextForParticle(nNode->getNextForParticle());
    }
    else {
        p->setHead(nNode->getNextForParticle());
    }

    if (nNode->getNext() != NULL) {
        nNode->getNext()->setPrevious(nNode->getPrevious());
    }
    else {
        tail = nNode->getPrevious();
    }
    if (nNode->getPrevious() != NULL) {
        nNode->getPrevious()->setNext(nNode->getNext());
    }
    else {
        head = nNode->getNext();
    }

    size--;
    return nNode;
}

Node* MultiQueue::popFirstInstance(Particle* p) {
    return popNthInstance(0, p);
}

Node* MultiQueue::getTimestepN(int n, Particle* p)
{
    Node* tNode = p->getHead();

    if (tNode == NULL) {
        return NULL;
    }

    while(tNode->getPosition()->getTimestep() < n && tNode->getNextForParticle() != NULL)
    {
        tNode = tNode->getNextForParticle();
    }

    return tNode;
}

Node* MultiQueue::popTimestepN(int n, Particle* p)
{
    Node* tNode = p->getHead();

    if (tNode == NULL) {
        return NULL;
    }

    while(tNode->getPosition()->getTimestep() < n && tNode->getNextForParticle() != NULL)
    {
        tNode = tNode->getNextForParticle();
        Node* noob = popFirstInstance(p);
        delete noob;
    }
    popFirstInstance(p);

    return tNode;
}

Node* MultiQueue::pop() {
    Particle* headParticle = head->getParticle();
    return popFirstInstance(headParticle);
}

// THIS PROBABLY DOESN'T WORK
void MultiQueue::pushHead(Node* n) {
    Node* particleHead = n->getParticle()->getHead();

    n->setNext(head);
    n->setNextForParticle(particleHead);

    if (head != NULL) {
        head->setPrevious(n);
    }
    if (particleHead != NULL) {
        particleHead->setPreviousForParticle(n);
    }

    head = n;
    size++;
}

void MultiQueue::pushTail(Node* n) {
    Node* particleTail = n->getParticle()->getTail();

    n->setPrevious(tail);
    n->setPreviousForParticle(particleTail);

    if (tail == NULL) {
        head = n;
        tail = n;
    }
    else {
        tail->setNext(n);
    }

    if (particleTail == NULL) {
        n->getParticle()->setHead(n);
        n->getParticle()->setTail(n);
    }
    else {
        particleTail->setNextForParticle(n);
    }

    tail = n;
    n->getParticle()->setTail(n);
    size++;
}
