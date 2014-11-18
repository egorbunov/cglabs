#include "Thorus.h"


CUSTOMVERTEX Thorus::getTorusVertex(float theta, float phi)
{
    return { 
        (majorRadius + minorRadius * cos(phi)) * cos(theta),
        (majorRadius + minorRadius * cos(phi)) * sin(theta),
        minorRadius * sin(phi),
        (DWORD) (phi * 0xffffff)
    };
}

Thorus::Thorus(float majorRadius, float minorRadius, D3DXVECTOR3 position) {
    this->majorRadius = majorRadius;
    this->minorRadius = minorRadius;
    float phi = 0;
    float theta = 0;
    float dalpha = D3DX_PI / (float) Thorus::FACTOR;
    int i = 0;
    for (float theta = 0; theta < 2 * D3DX_PI; theta += dalpha)
    {
        for (float phi = 0; phi < 2 * D3DX_PI; phi += dalpha)
        {
            vertices[i++] = getTorusVertex(theta, phi);
            vertices[i++] = getTorusVertex(theta + dalpha, phi);
            vertices[i++] = getTorusVertex(theta, phi + dalpha);
            vertices[i++] = getTorusVertex(theta + dalpha, phi);
            vertices[i++] = getTorusVertex(theta, phi + dalpha);
            vertices[i++] = getTorusVertex(theta + dalpha, phi + dalpha);
        }
    }
    
    this->translate(position.x, position.y, position.z);
}

void Thorus::render(LPDIRECT3DDEVICE9 d3dDevice) {
    d3dDevice->SetFVF(Thorus::CUSTMFVF);
    d3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));
    d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, Thorus::SIZE / 2);
}

void Thorus::create(LPDIRECT3DDEVICE9 d3dDevice) {
    // create a vertex buffer interface called v_buffer
    d3dDevice->CreateVertexBuffer(Thorus::SIZE * sizeof(CUSTOMVERTEX),
                                  0,
                                  Thorus::CUSTMFVF,
                                  D3DPOOL_MANAGED,
                                  &vertexBuffer,
                                  NULL);
    void* pVoid;
    // lock v_buffer and load the vertices into it
    vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices, sizeof(vertices));
    vertexBuffer->Unlock();
}