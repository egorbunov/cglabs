#ifndef ABSTRACT_RENDERABLE_OBJECT_H_INCLUDED__
#define ABSTRACT_RENDERABLE_OBJECT_H_INCLUDED__

#include <d3d9.h>
#include <d3dx9.h>

/*
    Pure virtual class which describes directx scene object that can be rendered
*/
class AbstractRenderableObject {
private:
    D3DXMATRIX worldMatrix;
    D3DXMATRIX translationMatrix;
    D3DXMATRIX rotationMatrix;
    D3DXMATRIX scaleMatrix;

    void updateWorldMatrix();
public:
    AbstractRenderableObject();

    void translate(float dx, float dy, float dz);

    void rotateX(float angle);

    void rotateY(float angle);

    void rotateZ(float angle);

    void scale(float sx, float sy, float sz);

    const D3DXMATRIX* getWorldMatrix();

    virtual void render() = 0;
};

#endif