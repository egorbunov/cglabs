#ifndef ABSTRACT_CAMERA_H_INCLUDED__
#define ABSTRACT_CAMERA_H_INCLUDED__

#include <d3d9.h>
#include <d3dx9.h>

class AbstractCamera {
public:
    virtual void moveLeft(float) = 0;
    virtual void moveRight(float) = 0;
    virtual void moveUp(float) = 0;
    virtual void moveDown(float) = 0;

    virtual D3DXVECTOR3 getPosition() = 0;
    virtual D3DXVECTOR3 getLookAt() = 0;
    virtual D3DXVECTOR3 getUpDirection() = 0;
    virtual const D3DMATRIX* getViewMatrix() = 0;

    virtual void setPosition(D3DXVECTOR3) = 0;
    virtual void setLookAt(D3DXVECTOR3) = 0;
    virtual void setUpDirection(D3DXVECTOR3) = 0;
};

#endif