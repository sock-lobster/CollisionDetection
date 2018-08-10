#include "boundingbox.h"

BoundingBox::BoundingBox(Particle* p)
{
    particle = p;
    radius = p->getRadius();
    Position* position = p->getCurPos();

    xMin = position->getX() - radius;
    xMax = position->getX() + radius;

    yMin = position->getY() - radius;
    yMax = position->getY() + radius;

    zMin = position->getZ() - radius;
    zMax = position->getZ() + radius;
}

BoundingBox::~BoundingBox(){}

Particle* BoundingBox::getParticle()
{
    return particle;
}

void BoundingBox::update()
{
    Position* position = particle->getCurPos();

    xMin = position->getX() - radius;
    xMax = position->getX() + radius;

    yMin = position->getY() - radius;
    yMax = position->getY() + radius;

    zMin = position->getZ() - radius;
    zMax = position->getZ() + radius;
}

double BoundingBox::getMax(int dimension)
{
    if (dimension == X_DIMENSION)
    {
        return xMax;
    }
    if (dimension == Y_DIMENSION)
    {
        return yMax;
    }
    if (dimension == Z_DIMENSION)
    {
        return zMax;
    }
}

double BoundingBox::getMin(int dimension)
{
    if (dimension == X_DIMENSION)
    {
        return xMin;
    }
    if (dimension == Y_DIMENSION)
    {
        return yMin;
    }
    if (dimension == Z_DIMENSION)
    {
        return zMin;
    }
}

double BoundingBox::getXMin(){
    return xMin;
}

double BoundingBox::getXMax(){
    return xMax;
}

double BoundingBox::getYMin(){
    return yMin;
}

double BoundingBox::getYMax(){
    return yMax;
}

double BoundingBox::getZMin(){
    return zMin;
}

double BoundingBox::getZMax(){
    return zMax;
}

bool BoundingBox::pointContained3D(double point[3]){
    for (int i = X_DIMENSION; i <= Z_DIMENSION; i++) {
        if (!(this->getMin(i) <= point[i] && this->getMax(i) >= point[i])) {
            return false;
        }
    }
    return true;
}

bool BoundingBox::pointContained2D(double point[2]){
    for (int i = X_DIMENSION; i <= Y_DIMENSION; i++) {
        if (!(this->getMin(i) <= point[i] && this->getMax(i) >= point[i])) {
            return false;
        }
    }
    return true;
}

bool BoundingBox::boxOverlap3D(BoundingBox* other) {
    // check other in this:
    // corner 1
    double corner1[3] = {other->getMin(X_DIMENSION), other->getMin(Y_DIMENSION), other->getMin(Z_DIMENSION)};
    if (pointContained3D(corner1)) {
        return true;
    }
    // corner 2
    double corner2[3] = {other->getMin(X_DIMENSION), other->getMin(Y_DIMENSION), other->getMax(Z_DIMENSION)};
    if (pointContained3D(corner2)) {
        return true;
    }
    // corner 3
    double corner3[3] = {other->getMin(X_DIMENSION), other->getMax(Y_DIMENSION), other->getMin(Z_DIMENSION)};
    if (pointContained3D(corner3)) {
        return true;
    }
    // corner 4
    double corner4[3] = {other->getMin(X_DIMENSION), other->getMax(Y_DIMENSION), other->getMax(Z_DIMENSION)};
    if (pointContained3D(corner4)) {
        return true;
    }
    // corner 5
    double corner5[3] = {other->getMax(X_DIMENSION), other->getMin(Y_DIMENSION), other->getMin(Z_DIMENSION)};
    if (pointContained3D(corner5)) {
        return true;
    }
    // corner 6
    double corner6[3] = {other->getMax(X_DIMENSION), other->getMin(Y_DIMENSION), other->getMax(Z_DIMENSION)};
    if (pointContained3D(corner6)) {
        return true;
    }
    // corner 7
    double corner7[3] = {other->getMax(X_DIMENSION), other->getMax(Y_DIMENSION), other->getMin(Z_DIMENSION)};
    if (pointContained3D(corner7)) {
        return true;
    }
    // corner 8
    double corner8[3] = {other->getMax(X_DIMENSION), other->getMax(Y_DIMENSION), other->getMax(Z_DIMENSION)};
    if (pointContained3D(corner8)) {
        return true;
    }

    // check other in this (we only need to check 1 corner).:
    // corner 1
    double corner9[3] = {this->getMin(X_DIMENSION), this->getMin(Y_DIMENSION), this->getMin(Z_DIMENSION)};
    if (other->pointContained3D(corner9)) {
        return true;
    }
    // corner 2
    double corner10[3] = {this->getMin(X_DIMENSION), this->getMin(Y_DIMENSION), this->getMax(Z_DIMENSION)};
    if (other->pointContained3D(corner10)) {
        return true;
    }
    // corner 3
    double corner11[3] = {this->getMin(X_DIMENSION), this->getMax(Y_DIMENSION), this->getMin(Z_DIMENSION)};
    if (other->pointContained3D(corner11)) {
        return true;
    }
    // corner 4
    double corner12[3] = {this->getMin(X_DIMENSION), this->getMax(Y_DIMENSION), this->getMax(Z_DIMENSION)};
    if (other->pointContained3D(corner12)) {
        return true;
    }
    // corner 5
    double corner13[3] = {this->getMax(X_DIMENSION), this->getMin(Y_DIMENSION), this->getMin(Z_DIMENSION)};
    if (other->pointContained3D(corner13)) {
        return true;
    }
    // corner 6
    double corner14[3] = {this->getMax(X_DIMENSION), this->getMin(Y_DIMENSION), this->getMax(Z_DIMENSION)};
    if (other->pointContained3D(corner14)) {
        return true;
    }
    // corner 7
    double corner15[3] = {this->getMax(X_DIMENSION), this->getMax(Y_DIMENSION), this->getMin(Z_DIMENSION)};
    if (other->pointContained3D(corner15)) {
        return true;
    }
    // corner 8
    double corner16[3] = {this->getMax(X_DIMENSION), this->getMax(Y_DIMENSION), this->getMax(Z_DIMENSION)};
    if (other->pointContained3D(corner16)) {
        return true;
    }
    return false;
}

bool BoundingBox::boxOverlap2D(BoundingBox* other) {
    // check other in this:
    // corner 1
    double corner1[2] = {other->getMin(X_DIMENSION), other->getMin(Y_DIMENSION)};
    if (pointContained2D(corner1)) {
        return true;
    }
    // corner 2
    double corner2[2] = {other->getMin(X_DIMENSION), other->getMax(Y_DIMENSION)};
    if (pointContained2D(corner2)) {
        return true;
    }
    // corner 3
    double corner3[2] = {other->getMax(X_DIMENSION), other->getMin(Y_DIMENSION)};
    if (pointContained2D(corner3)) {
        return true;
    }
    // corner 4
    double corner4[2] = {other->getMax(X_DIMENSION), other->getMax(Y_DIMENSION)};
    if (pointContained2D(corner4)) {
        return true;
    }

    // check other in this (we only need to check 1 corner).:
    // corner 1
    double corner5[2] = {this->getMin(X_DIMENSION), this->getMin(Y_DIMENSION)};
    if (other->pointContained2D(corner5)) {
        return true;
    }
    // corner 2
    double corner6[2] = {this->getMin(X_DIMENSION), this->getMax(Y_DIMENSION)};
    if (other->pointContained2D(corner6)) {
        return true;
    }
    // corner 3
    double corner7[2] = {this->getMax(X_DIMENSION), this->getMin(Y_DIMENSION)};
    if (other->pointContained2D(corner7)) {
        return true;
    }
    // corner 4
    double corner8[2] = {this->getMax(X_DIMENSION), this->getMax(Y_DIMENSION)};
    if (other->pointContained2D(corner8)) {
        return true;
    }
    return false;
}
