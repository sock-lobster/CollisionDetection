#ifndef QALGORITHM_H
#define QALGORITHM_H
#include <QObject>
#include "algorithm.h"

/**
 * @brief The QAlgorithm class is a QT wrapper for Algorithm.
 *
 * It provides an interface for the visualization related (Q*) components to
 * interact with the non-visualization related components.
 */
class QAlgorithm : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief QAlgorithm constructor calls the QObject constructor on its
     * parent if it has one. It also initializes the Algorithm it is to interact
     * with to the one given.
     * @param a Algorithm to wrap and interact with.
     * @param parent QObject parent if there is one.
     */
    explicit QAlgorithm(Algorithm* a, QObject *parent = 0);
    ~QAlgorithm();

    /**
     * @brief setAlgorithm setter for the Algorithm it is wrapping and
     * interacting with.
     * @param a new Algorithm to set.
     */
    void setAlgorithm(Algorithm* a);

    /**
     * @brief getAlgorithm getter for Algorithm currently wrapping.
     * @return Algorithm.
     */
    Algorithm* getAlgorithm();

    /**
     * @brief getParticles getter for the list of Particle's in the environment,
     * which the Algorithm algs knows about.
     * @return vector<Particle*> saved in Algorithm algs.
     */
    std::vector<Particle*> getParticles();

    /**
     * @brief getCenterOfGravity getter for the Particle that represetns
     * the center of gravity, which the Algorithm algs knows about.
     * @return Pointer to a Particle saved in Algorithm algs.
     */
    Particle* getCenterOfGravity();

    /**
     * @brief getSystemKE goes to the Algorithm algs to get the system kinetic energy.
     * @return A double representing the kinetic energy which is calculated by Algorithm algs.
     */
    double getSystemKE();

    /**
     * @brief getSystemPE goes to the Algorithm algs to get the system potential energy.
     * @return A double representing the potential energy which is calculated by Algorithm algs.
     */
    double getSystemPE();

signals:
    /**
     * @brief finished signal emited to begin the process of terminating the visualization and
     * simulation.
     */
    void finished();

public slots:
    /**
     * @brief runs the Algorithm algs and emits the signal finished() once algs is done.
     */
    void run();

    /**
     * @brief stops the Algorithm algs.
     */
    void stop();

private:
    Algorithm* alg;

};

#endif // QALGORITHM_H
