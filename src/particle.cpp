#include "particle.h"

Particle::Particle(int id, Position* startPos, std::valarray<double> startVel, double radiusIn, double massIn)
{
    this->id = id;
    vel = startVel;
    curPos = startPos;
    radius = radiusIn;
    mass = massIn;

    head = NULL;
    tail = NULL;
}

Particle::~Particle()
{
    delete curPos;
}

int Particle::getID()
{
    return id;
}

double Particle::getXvel()
{
    return vel[0];
}

double Particle::getYvel()
{
    return vel[1];
}

double Particle::getZvel()
{
    return vel[2];
}

double Particle::getXpos()
{
    return curPos->getPos()[0];
}

double Particle::getYpos()
{
    return curPos->getPos()[1];
}

double Particle::getZpos()
{
    return curPos->getPos()[2];
}

double Particle::getRadius()
{
    return radius;
}

double Particle::getMass()
{
    return mass;
}

Position* Particle::getCurPos()
{
    return curPos;
}

Node* Particle::getHead() {
    return head;
}

Node* Particle::getTail() {
    return tail;
}

void Particle::setHead(Node* n) {
    head = n;
}

void Particle::setTail(Node* n) {
    tail = n;
}

void Particle::updateCurPos(Position* newPos)
{
    delete curPos;
    curPos = newPos;
}

std::valarray<double> Particle::getVel()
{
    return vel;
}

void Particle::updateVel(double x, double y, double z)
{
    vel[0] = x;
    vel[1] = y;
    vel[2] = z;
}

std::valarray<double> Particle::getAcceleration()
{
    return acceleration;
}

void Particle::updateAcceleration(std::valarray<double> newA)
{
    acceleration = newA;
}
