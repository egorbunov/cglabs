#ifndef RANDOM_ROTATION_H_INCLUDED__
#define RANDOM_ROTATION_H_INCLUDED__

#include "TransformableObject.h"
#include <functional>
#include <random>
#include <vector>

class RandomRotation {
private:
    const float MAX_STEP = 0.002f;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> unifDist;

    float low;
    float high;
    float cur;
    float step;
    float minStep;

    std::vector<TransformableObject *> objects;
    std::vector<std::function<void(TransformableObject*, float val) > > callbacks;
public:
    RandomRotation(float low, float high, float minStep, float maxStep) : low(low), high(high), cur(low), 
        minStep(minStep), gen(rd()), unifDist(minStep, maxStep) {
        step = unifDist(gen);
    }

    void addTransformation(TransformableObject *obj, std::function<void(TransformableObject*, float )> callback) {
        objects.push_back(obj); 
        callbacks.push_back(callback);
    }

    void next() {
        cur += step;
        if (cur > high) {
            cur = high;
            step = -unifDist(gen);
        } else if (cur < low) {
            cur = low;
            step = unifDist(gen);
        }
        else {
            for (int i = 0; i < (int) objects.size(); ++i) {
                callbacks[i](objects[i], step);
            }
        }
    }
};

#endif