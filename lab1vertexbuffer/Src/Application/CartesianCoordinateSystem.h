#ifndef CARTESIAN_COORDINATE_SYSTEM_H_INCLUDED__
#define CARTESIAN_COORDINATE_SYSTEM_H_INCLUDED__

#include <cfloat>
#include "commonUtils.h"
#include "AbstractRenderableObject.h"


class CartesianCoordinateSystem : public AbstractRenderableObject {
private:
    static const int VERTEX_NUMBER = 6;
    static const int BIG_NUMBER = 500;

    D3DXVECTOR3 center;
    DWORD color;
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
    CUSTOMVERTEX axes[VERTEX_NUMBER];
public:
    ~CartesianCoordinateSystem();
    CartesianCoordinateSystem(D3DXVECTOR3 center, DWORD color, float length = (float) BIG_NUMBER);
    void create(LPDIRECT3DDEVICE9 d3dDevice);
    void render(LPDIRECT3DDEVICE9 d3dDevice) override;
};

#endif