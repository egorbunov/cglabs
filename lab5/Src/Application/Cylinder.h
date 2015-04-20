#ifndef CYLINDER_H_INCLUDED__
#define CYLINDER_H_INCLUDED__

#include <vector>
#include <d3dx9.h>
#include <stdexcept>

#include "RenderableObject.h"


class Cylinder : public RenderableObject {
private:
    struct VertexWithNormal { D3DXVECTOR3 p; D3DVECTOR NORMAL; DWORD COLOR; };

    D3DXVECTOR3 center;
    DWORD color;
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
    std::vector<VertexWithNormal> vertices;
    LPDIRECT3DDEVICE9 d3dDevice;

public:
    static const int FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;

    /*
    * @param r - radius
    * @param h - height
    * @param color - cyl. color
    */
    Cylinder(LPDIRECT3DDEVICE9 d3dDevice, float r, float h, DWORD color);

    ~Cylinder();

    void render(const Transform *worldTransform);
};

#endif