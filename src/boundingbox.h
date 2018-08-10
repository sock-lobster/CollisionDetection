#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include "particle.h"
#include "constants.h"

/**
 * @brief The BoundingBox class represents a square or cube that fits perfectly
 * around a Particle.
 *
 * This class is mostly a convenient way of tracking the endpoints or extremes
 * of a Particle in each dimension.
 */
class BoundingBox
{
public:
    /**
     * @brief Constructs a BoundingBox around the given Particle.
     *
     * This tells the BoundingBox what Particle it is responsible for, and sets
     * the appropriate minimum and maximum values for each dimension.
     *
     * @param p the Particle this BoundingBox is responsible for.
     */
    BoundingBox(Particle *p);

    ~BoundingBox();

    /**
     * @brief Returns the Particle this BouningBox is tracking the endpoints of.
     * @return The Particle this BoundingBox tracks the endpoinst of.
     */
    Particle* getParticle();

    /**
     * @brief Updates the position of the BouningBox to reflect the Particle.
     *
     * This sets the minimum and maximum for each dimension to be the
     * appropriate value given the current Position of the Particle this
     * BouningBox is keeping track of.
     */
    void update();

    /**
     * @brief Returns the maximum of the desired dimension
     *
     * This returns the maximum coordinate of this BoundingBox in the desired
     * dimension. This is mostly a glorified and generalized getter.
     *
     * @param dimension the desired from: X_DIMENSION, Y_DIMENSION, Z_DIMENSION.
     * @return The maximum of the desired dimension.
     */
    double getMax(int dimension);

    /**
     * @brief Returns the minimum of the desired dimension
     *
     * This returns the minimum coordinate of this BoundingBox in the desired
     * dimension. This is mostly a glorified and generalized getter.
     *
     * @param dimension the desired from: X_DIMENSION, Y_DIMENSION, Z_DIMENSION.
     * @return The minimum of the desired dimension.
     */
    double getMin(int dimension);

    /**
     * @brief Returns the minimum x dimension coordinate of this BoundingBox.
     * @return The minimum x coordinate this BoundingBox's Particle touches.
     */
    double getXMin();

    /**
     * @brief Returns the maximum x dimension coordinate of this BouningBox.
     * @return The maximum x coordinate this BouningBox's Particle touches.
     */
    double getXMax();

    /**
     * @brief Returns the minimum y dimension coordinate of this BoundingBox.
     * @return The minimum y coordinate this BoundingBox's Particle touches.
     */
    double getYMin();

    /**
     * @brief Returns the maximum y dimension coordinate of this BouningBox.
     * @return The maximum y coordinate this BouningBox's Particle touches.
     */
    double getYMax();

    /**
     * @brief Returns the minimum z dimension coordinate of this BoundingBox.
     * @return The minimum z coordinate this BoundingBox's Particle touches.
     */
    double getZMin();

    /**
     * @brief Returns the maximum z dimension coordinate of this BouningBox.
     * @return The maximum z coordinate this BouningBox's Particle touches.
     */
    double getZMax();

    /**
     * @brief Returns whether a point in 3-space falls within this BoundingBox.
     *
     * Ensures the x coordinate of the point lies between this BouningBox's
     * x minimum and x maximum, and does the same for the y and z dimensions.
     *
     * @param point a point in 3-space.
     * @return True if the point lies in this BoundingBox, false otherwise.
     */
    bool pointContained3D(double point[3]);

    /**
     * @brief Returns whether or not 2 BoundingBox%es overlap (in 3-space).
     *
     * If any corner of either of the BoundingBox%es lies within the other
     * BoundingBox, then they overlap, if this is true for no corners, then
     * they do not overlap. This requires us to check all 16 corners.
     *
     * @param other the BouningBox to check against this one for overlap.
     * @return True if the BoundingBox%es overlap, false otherwise.
     */
    bool boxOverlap3D(BoundingBox* other);

    /**
     * @brief Returns whether a point in 2-space falls within this BoundingBox.
     *
     * Ensures the x coordinate of the point lies between this BouningBox's
     * x minimum and x maximum, and does the same for the y dimension.
     *
     * @param point a point in 2-space.
     * @return True if the point lies in this BoundingBox, false otherwise.
     */
    bool pointContained2D(double point[2]);

    /**
     * @brief Returns whether or not 2 BoundingBox%es overlap (in 3-space).
     *
     * If any corner of either of the BoundingBox%es lies within the other
     * BoundingBox, then they overlap, if this is true for no corners, then
     * they do not overlap. This requires us to check all 16 corners.
     *
     * @param other the BouningBox to check against this one for overlap.
     * @return True if the BoundingBox%es overlap, false otherwise.
     */
    bool boxOverlap2D(BoundingBox* other);

private:
    Particle* particle;
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double zMin;
    double zMax;
    double radius;
};

#endif // BOUNDINGBOX_H
