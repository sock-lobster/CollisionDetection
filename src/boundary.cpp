#include "boundary.h"

Boundary::Boundary(int dimensionIn, double valueIn, bool isUpper, BoundingBox *boxIn)
{
    dimension = dimensionIn;
    value = valueIn;
    upper = isUpper;
    box = boxIn;
}

Boundary::~Boundary(){}

double Boundary::getValue()
{
    return value;
}

bool Boundary::isUpper()
{
    return upper;
}

bool Boundary::isLower()
{
    return !upper;
}

int Boundary::getDimension()
{
    return dimension;
}

BoundingBox *Boundary::getBox()
{
    return box;
}

void Boundary::update()
{
    if (upper)
    {
        value = box->getMax(dimension);
    }
    else
    {
        value = box->getMin(dimension);
    }
}

