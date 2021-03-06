#include "CartesianCoordinateSystem.h"

CartesianCoordinateSystem::CartesianCoordinateSystem(D3DXVECTOR3 center, DWORD color, float length) {
    vertexBuffer = NULL;
    this->center = center;
    this->translate(center.x, center.y, center.z);
    for (int i = 0; i < VERTEX_NUMBER; ++i) {
        axes[i] = {0.0f, 0.0f, 0.0f, color};
    }
    axes[1].X = axes[3].Y = axes[5].Z = length;
}

void CartesianCoordinateSystem::render(LPDIRECT3DDEVICE9 d3dDevice) {
    d3dDevice->SetFVF(CUSTOMFVF);
    d3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
    d3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, VERTEX_NUMBER / 2);
}

void CartesianCoordinateSystem::create(LPDIRECT3DDEVICE9 d3dDevice) {
    d3dDevice->CreateVertexBuffer(VERTEX_NUMBER * sizeof(CUSTOMVERTEX),
                                  D3DUSAGE_WRITEONLY,
                                  CUSTOMFVF,
                                  D3DPOOL_MANAGED,
                                  &vertexBuffer,
                                  NULL);
    void* pVoid;
    // lock v_buffer and load the vertices into it
    vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, axes, sizeof(axes));
    vertexBuffer->Unlock();
}

CartesianCoordinateSystem::~CartesianCoordinateSystem() {
    if (vertexBuffer != NULL) {
        vertexBuffer->Release();
    }
}