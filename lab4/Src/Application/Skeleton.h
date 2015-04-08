#ifndef SKELETON_H_INCLUDED__
#define SKELETON_H_INCLUDED__

#include <vector>

#include "RenderableObject.h"

class Skeleton : public RenderableObject {
    std::vector<Skeleton*> children;
    RenderableObject *object;

    void render(const Transform *);
public:
    Skeleton(RenderableObject *object);
    ~Skeleton();

    void addChild(Skeleton *child);
    void clean();
};

#endif