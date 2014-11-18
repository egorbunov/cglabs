#ifndef CAMERA_H_INCLUDED__
#define CAMERA_H_INCLUDED__

#include <cmath>
#include <d3d9.h>
#include <d3dx9.h>

class Camera {
private:
    D3DXMATRIX viewMatrix;
    D3DXMATRIX translateMatrix;
    D3DXVECTOR3 upDirection;
    float radius;
    float zenith;
    float azimuth;

    void updateView();
    void flipNormalAndD3DCoord(D3DXVECTOR3 *p);
public:
    Camera(D3DXVECTOR3 position, D3DXVECTOR3 lookAt, D3DXVECTOR3 upDirection);
    const D3DMATRIX* getViewMatrix();

    void zoom(float shift);
    void rotateZenith(float dtheta);
    void rotateAzimuth(float dtheta);
};

#endif