#ifndef BOUNDARY_H
#define BOUNDARY_H
#include "boundingbox.h"

/**
 * @brief The Boundary class is an object representing a Particle's endpoints.
 */
class Boundary
{
public:
    /**
     * @brief Constructs a Boundary object.
     *
     * Constructs a Boundary object representing a side or face of a BoundingBox.
     * Initializes the dimension the Boundary is in, the location of the
     * Boundary, whether it is an upper or lower bound, and the BoundingBox
     * that this Boundary of the end of.
     *
     * @param dimensionIn either X_DIMENSION, Y_DIMENSION, or Z_DIMENSION.
     * @param valueIn the location of this Boundary in the dimension.
     * @param isUpper true if this represents the max of a BoundingBox in the
     * dimension.
     * @param boxIn the BoundingBox we are representing a border of.
     */
    Boundary(int dimensionIn, double valueIn, bool isUpper, BoundingBox* boxIn);

    ~Boundary();

    /**
     * @brief Returns the coordinate of this Boundary.
     * @return The coordinate of this Boundary.
     */
    double getValue();

    /**
     * @brief Returns whether or not this is an upper bound of a BoundingBox.
     * @return True if it is an upper bound, False if not.
     */
    bool isUpper();

    /**
     * @brief Returns whether or not this is a lower bound of a BoundingBox.
     * @return True if it is a lower bound, False if not.
     */
    bool isLower();

    /**
     * @brief Returns the dimension this Boundary is bounding
     * @return Either X_DIMENSION, Y_DIMENSION, or Z_DIMENSION.
     */
    int getDimension();

    /**
     * @brief Returns a pointer to the BoundingBox this Boundary relates to.
     * @return The associated BoundingBox.
     */
    BoundingBox* getBox();

    /**
     * @brief Adjusts the coordinte value of this Bounary to reflect the
     * current location of its associated BoundingBox.
     */
    void update();

private:
    double value;
    bool upper;
    int dimension;
    BoundingBox* box;
};


#endif // BOUNDARY_H
