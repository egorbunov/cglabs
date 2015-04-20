#ifndef RENDERABLE_OBJECT_H_INCLUDED__
#define RENDERABLE_OBJECT_H_INCLUDED__

#include "TransformableObject.h"

class RenderableObject : public TransformableObject {
public:
    virtual ~RenderableObject() = 0 {};
    virtual void render(const Transform *worldTransform) = 0;
};

#endif