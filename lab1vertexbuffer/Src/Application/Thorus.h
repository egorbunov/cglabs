#ifndef THORUS_H_INCLUDED__
#define THORUS_H_INCLUDED__

#include "AbstractRenderableObject.h"
#include "commonUtils.h"

class Thorus : public AbstractRenderableObject {
private:
    static const int FACTOR = 40;
    static const int SIZE = 24 * Thorus::FACTOR * Thorus::FACTOR;
    static const int CUSTMFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

    CUSTOMVERTEX vertices[Thorus::SIZE];

    float majorRadius;
    float minorRadius;
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer;

    CUSTOMVERTEX getTorusVertex(float theta, float phi);
public:
    Thorus(float majorRadius, float minorRadius, D3DXVECTOR3 position);
    void create(LPDIRECT3DDEVICE9 d3dDevice);
    void render(LPDIRECT3DDEVICE9 d3dDevice) override;
};

#endif