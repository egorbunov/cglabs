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

    virtual TransformableObject* translate(float dx, float dy, float dz);

    virtual TransformableObject* rotateX(float angle);

    virtual TransformableObject* rotateY(float angle);

    virtual TransformableObject* rotateZ(float angle);

    virtual TransformableObject* scale(float sx, float sy, float sz);

    const D3DXMATRIX* getWorldMatrix();
    const D3DXMATRIX* getTranslationMatrix();
    const D3DXMATRIX* getScaleMatrix();
    const D3DXMATRIX* getRotationMatrix();

};

#endif