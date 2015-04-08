#include "CircleIterator.h"

#include <cmath>


CircleIterator::CircleIterator(float radius, float initialAngle, float angleStep, bool cw) {
    this->radius = radius;
    this->angle = initialAngle;
    this->dphi = angleStep;
    this->cw = 1;
    if (cw) {
        this->cw = -1;
    }
}

float CircleIterator::getAngleStep() const {
    return dphi;
}

float CircleIterator::getXTranslate() const {
    return dx;
}

float CircleIterator::getZTranslate() const {
    return dz;
}

void CircleIterator::next() {
    dx = -radius * sin(angle);
    dz = radius * cos(angle);
    angle = angle + cw * dphi;
    if (angle > 0.0) {
        angle = angle = (float) (2.0 * MY_PI);
    }
    dx = -radius * sin(angle) - dx;
    dz = radius * cos(angle) - dz;

    for (int i = 0; i < (int) listeners.size(); ++i) {
        listeners[i]->rotateY(isClockwise() * getAngleStep());
        if (!isOnlyRotate[i])
            listeners[i]->translate(getXTranslate(), 0, getZTranslate());
    }
}

float CircleIterator::isClockwise() const {
    return -cw;
}

void CircleIterator::addListener(TransformableObject *obj, bool onlyToRotate) {
    listeners.push_back(obj);
    isOnlyRotate.push_back(onlyToRotate);
}