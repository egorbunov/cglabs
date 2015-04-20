#include "Cylinder.h"


Cylinder::Cylinder(LPDIRECT3DDEVICE9 d3dDevice, float r, float h, DWORD color) {
    VertexWithNormal vlt, vrt, vlb, vrb;

    int n = 60;
    float step = 1.0f / n;


    for (int i = 0; i < n; ++i) { // cart. z-axis
        for (int j = 0; j < n; ++j) { // phi-angle
            D3DVECTOR pnt = { r * cos(j * step * D3DX_PI * 2), r * sin(j * step * D3DX_PI * 2), i * step * h - h / 2.0f };
            D3DVECTOR norm = { pnt.x / r, pnt.y / r, 0 };
            vrb = { { pnt.x, pnt.y, pnt.z }, norm, color };

            pnt = { r * cos((j + 1) * step * D3DX_PI * 2), r * sin((j + 1) * step * D3DX_PI * 2), i * step * h - h / 2.0f };
            norm = { pnt.x / r, pnt.y / r, 0 };
            vrt = { { pnt.x, pnt.y, pnt.z }, norm, color };

            pnt = { r * cos(j * step * D3DX_PI * 2), r * sin(j * step * D3DX_PI * 2), (i + 1) * step * h - h / 2.0f };
            norm = { pnt.x / r, pnt.y / r, 0 };
            vlb = { { pnt.x, pnt.y, pnt.z }, norm, color };

            pnt = { r * cos((j + 1) * step * D3DX_PI * 2), r * sin((j + 1) * step * D3DX_PI * 2), (i + 1) * step * h - h / 2.0f };
            norm = { pnt.x / r, pnt.y / r, 0 };
            vlt = { { pnt.x, pnt.y, pnt.z }, norm, color };

            vertices.push_back(vrb);
            vertices.push_back(vlb);
            vertices.push_back(vrt);

            vertices.push_back(vrt);
            vertices.push_back(vlt);
            vertices.push_back(vlb);

        }
    }

    /*
    const D3DXMATRIX *wMat = getWorldTransfrom()->getTransformMatrix();
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        D3DXVec3TransformCoord(&vertices[i].p, &vertices[i].p, wMat);
    }

    getWorldTransfrom()->reset();
    */


    /*
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        D3DXVec3TransformCoord(&vertices[i].p, &vertices[i].p, getWorldTransfrom()->getTransformMatrix());
    }
    */

    this->d3dDevice = d3dDevice;
    d3dDevice->CreateVertexBuffer(vertices.size() * sizeof(VertexWithNormal),
                                  D3DUSAGE_WRITEONLY,
                                  FVF,
                                  D3DPOOL_DEFAULT,
                                  &vertexBuffer,
                                  NULL);
    void* pVoid;
    // lock v_buffer and load the vertices into it
    vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices.data(), vertices.size() * sizeof(VertexWithNormal));
    vertexBuffer->Unlock();
}

Cylinder::~Cylinder() {
    if (vertexBuffer != NULL)
        vertexBuffer->Release();
}

void Cylinder::render(const Transform *worldTransform) {
    if (d3dDevice == NULL) {
        throw std::runtime_error("LPDIRECT3DEVICE is null! Bad constructor param...");
    }
    d3dDevice->SetFVF(FVF);
    d3dDevice->SetRenderState(D3DRS_LIGHTING, true);
    d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    d3dDevice->SetTransform(D3DTS_WORLD, worldTransform->getTransformMatrix());

    d3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(VertexWithNormal));
    d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertices.size() / 3);
}