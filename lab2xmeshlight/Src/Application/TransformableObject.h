#ifndef ABSTRACT_RENDERABLE_OBJECT_H_INCLUDED__
#define ABSTRACT_RENDERABLE_OBJECT_H_INCLUDED__

#include <d3d9.h>
#include <d3dx9.h>

/*
    
*/
class TransformableObject {
protected:
    D3DXMATRIX worldMatrix;
    D3DXMATRIX translationMatrix;
    D3DXMATRIX rotationMatrix;
    D3DXMATRIX scaleMatrix;

    void updateWorldMatrix();
public:
    TransformableObject();

    TransformableObject* translate(float dx, float dy, float dz);

    TransformableObject* rotateX(float angle);

    TransformableObject* rotateY(float angle);

    TransformableObject* rotateZ(float angle);

    TransformableObject* scale(float sx, float sy, float sz);

    const D3DXMATRIX* getWorldMatrix();
};

#endif