#ifndef CIRCLE_ITERATOR_H_INCLUDED__
#define CIRCLE_ITERATOR_H_INCLUDED__

#define MY_PI 3.1415926535897

#include <vector>
#include "TransformableObject.h"

class CircleIterator {
private:
    float angle;
    float radius;
    float dphi;
    float x;
    float z;
    float dx;
    float dz;
    float cw;

    std::vector<TransformableObject*> listeners;
    std::vector<bool> isOnlyRotate;
public:
    CircleIterator(float radius, float initialAngle, float angleStep, bool cw = true);
    float getAngleStep() const;
    float getXTranslate() const;
    float getZTranslate() const;
    float isClockwise() const;

    void next();

    void addListener(TransformableObject *obj, bool onlyToRotate = false);
    
};

#endif