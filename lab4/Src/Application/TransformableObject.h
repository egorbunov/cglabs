#ifndef ABSTRACT_RENDERABLE_OBJECT_H_INCLUDED__
#define ABSTRACT_RENDERABLE_OBJECT_H_INCLUDED__

#include <d3d9.h>
#include <d3dx9.h>
#include "Transform.h"

class TransformableObject {
    void updateWorldMat();

protected:
    Transform worldTransform;

public:
    TransformableObject();

    const Transform *getWorldTransfrom();

    const D3DXMATRIX* getTransformMatrix();

    void rotateX(float phi);
    void rotateY(float phi);
    void rotateZ(float phi);
    void rotateAxis(float ax, float ay, float az, float phi);
    void rotateAxis(D3DXVECTOR3 &axis, float phi);


    void transform(Transform &transform);


    void translate(float dx, float dy, float dz);

    void scale(float sx, float sy, float sz);
};

#endif