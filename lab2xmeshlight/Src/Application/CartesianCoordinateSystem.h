#ifndef CARTESIAN_COORDINATE_SYSTEM_H_INCLUDED__
#define CARTESIAN_COORDINATE_SYSTEM_H_INCLUDED__

#include <stdexcept>
#include "TransformableObject.h"

class CartesianCoordinateSystem : public TransformableObject {
private:
    struct SimpleVertex { FLOAT X, Y, Z; DWORD COLOR; };

    static const int VERTEX_NUMBER = 6;

    D3DXVECTOR3 center;
    DWORD color;
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
    SimpleVertex axes[VERTEX_NUMBER];
    LPDIRECT3DDEVICE9 d3dDevice;
public:
    static const int FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

    CartesianCoordinateSystem(D3DXVECTOR3 center, DWORD color);
    void create(LPDIRECT3DDEVICE9 d3dDevice);
    void render();
};

#endif