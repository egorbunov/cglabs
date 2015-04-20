#ifndef TRANSFORM_H_INCLUDED__
#define TRANSFORM_H_INCLUDED__

#include <d3d9.h>
#include <d3dx9.h>

class Transform {
protected:
    D3DXMATRIX matrix;

    void updateMatrix();
public:
    Transform() {
        D3DXMatrixIdentity(&matrix);
    }

    virtual Transform* reset() {
        D3DXMatrixIdentity(&matrix);
        return this;
    }

    virtual void translate(float dx, float dy, float dz);

    virtual void rotateX(float angle);

    virtual void rotateY(float angle);

    virtual void rotateZ(float angle);

    virtual void scale(float sx, float sy, float sz);

    virtual void rotateAxis(float ax, float ay, float az, float phi);


    virtual void transform(const Transform &transform);

    const D3DXMATRIX* getTransformMatrix() const;

    Transform(const Transform &obj);

    static Transform getInverse(const Transform &t);
};

#endif