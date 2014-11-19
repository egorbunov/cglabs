#ifndef CIRCLE_ITERATOR_H_INCLUDED__
#define CIRCLE_ITERATOR_H_INCLUDED__

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
public:
    CircleIterator(float radius, float initialAngle, float angleStep, bool cw = true);
    float getAngleStep() const;
    float getXTranslate() const;
    float getZTranslate() const;
    float isClockwise() const;
    void next();
    
};

#endif