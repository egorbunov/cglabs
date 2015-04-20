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
    D3DXVECTOR3 pos;
    D3DXVECTOR3 lookAt;

    float radius;
    float zenith;
    float azimuth;

    bool needToChangeDirection;

    void updateView();
    void flipNormalAndD3DCoord(D3DXVECTOR3 *p);

public:
    Camera();

    void zoom(float shift);
    void rotateZenith(float dtheta);
    void rotateAzimuth(float dtheta);

    D3DXVECTOR3 getPosition();
    D3DXVECTOR3 getLookAt();
    D3DXVECTOR3 getUpDirection();
    const D3DMATRIX* getViewMatrix();

    void setPosition(D3DXVECTOR3);
    void setLookAt(D3DXVECTOR3);
    void setUpDirection(D3DXVECTOR3);

    inline float getZoom() const {
        return radius;
    }
};

#endif