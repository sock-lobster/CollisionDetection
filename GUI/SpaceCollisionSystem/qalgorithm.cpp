#include "qalgorithm.h"

QAlgorithm::QAlgorithm(Algorithm* a, QObject *parent) :
    QObject(parent)
{
    alg = a;
}

QAlgorithm::~QAlgorithm()
{
    delete alg;
}

std::vector<Particle*> QAlgorithm::getParticles()
{
    return alg->getParticles();
}

Particle* QAlgorithm::getCenterOfGravity()
{
    return alg->getCenterOfGravity();
}

void QAlgorithm::setAlgorithm(Algorithm *a)
{
    alg = a;
}

void QAlgorithm::run()
{
    alg->run();
    emit finished();
}

void QAlgorithm::stop()
{
    alg->stop();
}

Algorithm* QAlgorithm::getAlgorithm()
{
    return alg;
}

double QAlgorithm::getSystemKE()
{
    return alg->getSystemKE();
}

double QAlgorithm::getSystemPE()
{
    return alg->getSystemPE();
}
