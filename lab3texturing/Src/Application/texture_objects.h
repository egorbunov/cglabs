#ifndef TEXTURED_OBJECT_H_INCLUDED__
#define TEXTURED_OBJECT_H_INCLUDED__

#include <vector>
#include <string>

#include "TransformableObject.h"

#define TEXTURE_VERTEX_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

struct TEXTURE_VERTEX_WITH_NORMAL { FLOAT X, Y, Z; D3DVECTOR NORMAL; FLOAT U, V; };


class TexturedObject : public TransformableObject {
public:
    TexturedObject(LPDIRECT3DDEVICE9);
    ~TexturedObject();
    void render();
    void prepare();
    void setVertices(const std::vector<TEXTURE_VERTEX_WITH_NORMAL>);

protected:
    int triangleCount;
    LPDIRECT3DVERTEXBUFFER9 vbuffer;
    IDirect3DTexture9 *pTexture;
    LPDIRECT3DDEVICE9 d3dDevice;

};

// Textured rectangle
class TexturedSquare : public TexturedObject {
public:
    TexturedSquare(LPDIRECT3DDEVICE9 device, const D3DXVECTOR3& normal, const D3DXVECTOR3& shift, float size, std::vector<LPCWSTR>);
};

#endif