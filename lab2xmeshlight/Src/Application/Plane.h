#ifndef PLANE_H_INCLUDED__
#define PLANE_H_INCLUDED__

#include "TransformableObject.h"
#include <d3dx9.h>
#include <stdexcept>

class Plane : public TransformableObject {
private:
    struct VertexWithNormal { FLOAT X, Y, Z; D3DVECTOR NORMAL; DWORD COLOR; };
    struct SimpleVertex { FLOAT X, Y, Z; DWORD COLOR; };

    static const int VERTEX_NUMBER = 6;
    static const int BIG_NUMBER = 500;

    D3DXVECTOR3 center;
    DWORD color;
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
    VertexWithNormal vertices[VERTEX_NUMBER];
    LPDIRECT3DDEVICE9 d3dDevice;
public:
    static const int FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

    Plane(D3DXVECTOR3 center, D3DVECTOR normal, float scaleFactor) {
        vertices[0] = { 1, -normal.x / normal.y, 0, normal, 0x0000ff };
        vertices[1] = { 0, -normal.z / normal.y, 1, normal, 0x0000ff };
        vertices[2] = { -1, normal.x / normal.y, 0, normal, 0x0000ff };
        vertices[3] = { 1, -normal.x / normal.y, 0, normal, 0x0000ff };
        vertices[4] = { -1, normal.x / normal.y, 0, normal, 0x0000ff };
        vertices[5] = { 0, -normal.z / normal.y, -1, normal, 0x0000ff };

        scale(scaleFactor, scaleFactor, scaleFactor);
    }

    void create(LPDIRECT3DDEVICE9 d3dDevice) {
        this->d3dDevice = d3dDevice;
        d3dDevice->CreateVertexBuffer(VERTEX_NUMBER * sizeof(VertexWithNormal),
                                      0,
                                      FVF,
                                      D3DPOOL_MANAGED,
                                      &vertexBuffer,
                                      NULL);
        void* pVoid;
        // lock v_buffer and load the vertices into it
        vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
        memcpy(pVoid, vertices, sizeof(vertices));
        vertexBuffer->Unlock();
    }
    void render() {
        if (d3dDevice == NULL) {
            throw std::runtime_error("LPDIRECT3DEVICE is null! Probably you does't call create(...) method!");
        }
        d3dDevice->SetFVF(FVF);
        d3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(VertexWithNormal));
        d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, VERTEX_NUMBER / 3);
    }
};

#endif