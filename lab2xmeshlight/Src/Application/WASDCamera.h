#ifndef WASD_CAMERA_H_INCLUDED__
#define WASD_CAMERA_H_INCLUDED__

#include <d3d9.h>
#include <d3dx9.h>


class WASDCamera {
    D3DXMATRIX viewMatrix;
    D3DXMATRIX translateMatrix;
    D3DXMATRIX rotationMatrix;

    D3DXVECTOR3 upDirection;
    D3DXVECTOR3 lookAt;
    D3DXVECTOR3 pos;
public:
    WASDCamera();

    void setUpDirection(D3DXVECTOR3 upDirection);
    void setLookAt(D3DXVECTOR3 lookAt);
    void setPosition(D3DXVECTOR3 pos);

    D3DXVECTOR3 getPosition() const;
    D3DXVECTOR3 getUpDirection() const;
    D3DXVECTOR3 getLookAt() const;

    void rotate(float dx, float dy);
    void moveLeft(float shift);
    void moveRight(float shift);
    void moveForward(float shift);
    void moveBackward(float shift);

    const D3DMATRIX* getViewMatrix();

};

#endif