#ifndef PLANE_H_INCLUDED__
#define PLANE_H_INCLUDED__

#include "TransformableObject.h"
#include <d3dx9.h>
#include <stdexcept>

class Plane : public TransformableObject {
private:
    struct VertexWithNormal { FLOAT X, Y, Z; D3DVECTOR NORMAL; DWORD COLOR; };

    static const int BIG_NUMBER = 500;

    D3DXVECTOR3 center;
    DWORD color;
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
    VertexWithNormal *vertices;
    LPDIRECT3DDEVICE9 d3dDevice;
    int vertexNumber;
public:
    static const int FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

    Plane(D3DXVECTOR3 center, D3DVECTOR normal, float scaleFactor, float step = 0.2) {
        vertexNumber = 0;
        VertexWithNormal vlt, vrt, vlb, vrb;

        vertices = new VertexWithNormal[(size_t)(6 * (2 / step + 1) * (2 / step + 1)) + 1];
        for (float x = -1; x < 1; x += step) {
            for (float z = -1; z < 1; z += step) {
                vlt = { x, (-x * normal.x) / normal.y, z, normal, 0xff00ffff };
                vrt = { x + step, (-(x + step) * normal.x) / normal.y, z, normal, 0xff00ffff };
                vlb = { x, (-x * normal.x) / normal.y, z + step, normal, 0xff00ffff };
                vrb = { x + step, (-(x + step) * normal.x) / normal.y, z + step, normal, 0xff00ffff };

                vertices[vertexNumber++] = vlt;
                vertices[vertexNumber++] = vrt;
                vertices[vertexNumber++] = vlb;
                vertices[vertexNumber++] = vlb;
                vertices[vertexNumber++] = vrb;
                vertices[vertexNumber++] = vrt;
            }
        }

        translate(center.x, center.y, center.z);
        scale(scaleFactor, scaleFactor, scaleFactor);
    }

    void create(LPDIRECT3DDEVICE9 d3dDevice) {
        this->d3dDevice = d3dDevice;
        d3dDevice->CreateVertexBuffer(vertexNumber * sizeof(VertexWithNormal),
                                      0,
                                      FVF,
                                      D3DPOOL_MANAGED,
                                      &vertexBuffer,
                                      NULL);
        void* pVoid;
        // lock v_buffer and load the vertices into it
        vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
        memcpy(pVoid, vertices, vertexNumber * sizeof(VertexWithNormal));
        vertexBuffer->Unlock();
    }
    void render() {
        if (d3dDevice == NULL) {
            throw std::runtime_error("LPDIRECT3DEVICE is null! Probably you does't call create(...) method!");
        }
        d3dDevice->SetFVF(FVF);
        d3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(VertexWithNormal));
        d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertexNumber / 3);
    }
};

#endif