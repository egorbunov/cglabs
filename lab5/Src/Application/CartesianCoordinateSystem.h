#ifndef CARTESIAN_COORDINATE_SYSTEM_H_INCLUDED__
#define CARTESIAN_COORDINATE_SYSTEM_H_INCLUDED__

#include <stdexcept>
#include "RenderableObject.h"

class CartesianCoordinateSystem : public RenderableObject {
private:
    struct SimpleVertex { FLOAT X, Y, Z; DWORD COLOR; };

    static const int VERTEX_NUMBER = 6;

    DWORD color;
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
    SimpleVertex axes[VERTEX_NUMBER];
    LPDIRECT3DDEVICE9 d3dDevice;
public:
    static const int FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
    ~CartesianCoordinateSystem();
    CartesianCoordinateSystem(LPDIRECT3DDEVICE9 d3dDevice, DWORD color);
    void render(const Transform *transform);
};

#endif