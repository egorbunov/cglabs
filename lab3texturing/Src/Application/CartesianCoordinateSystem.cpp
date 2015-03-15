#include "CartesianCoordinateSystem.h"


CartesianCoordinateSystem::CartesianCoordinateSystem(D3DXVECTOR3 center, DWORD color) {
    this->vertexBuffer = NULL;
    this->d3dDevice = NULL;
    this->center = center;
    this->translate(center.x, center.y, center.z);
    for (int i = 0; i < VERTEX_NUMBER; ++i) {
        axes[i] = {0.0f, 0.0f, 0.0f, color};
    }
    axes[1].X = axes[3].Y = axes[5].Z = 1.0f;
}

void CartesianCoordinateSystem::render() {
    if (d3dDevice == NULL) {
        throw std::runtime_error("LPDIRECT3DEVICE is null! Probably you does't call create(...) method!");
    }
    d3dDevice->SetFVF(CartesianCoordinateSystem::FVF);
    d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    d3dDevice->SetTransform(D3DTS_WORLD, this->getWorldMatrix());
    d3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(SimpleVertex));
    d3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, VERTEX_NUMBER / 2);
}

/*
    creates buffer and sets device
*/
void CartesianCoordinateSystem::create(LPDIRECT3DDEVICE9 d3dDevice) {
    this->d3dDevice = d3dDevice;
    d3dDevice->CreateVertexBuffer(VERTEX_NUMBER * sizeof(SimpleVertex),
                                  D3DUSAGE_WRITEONLY,
                                  CartesianCoordinateSystem::FVF,
                                  D3DPOOL_DEFAULT,
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