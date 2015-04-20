#include "texture_objects.h"

// Base class implementation **************************

TexturedObject::TexturedObject(LPDIRECT3DDEVICE9 device) {
    d3dDevice = device;
    vbuffer = NULL;
    pTexture = NULL;
    triangleCount = 0;
}

TexturedObject::~TexturedObject() {
    vbuffer->Release();
    pTexture->Release();
}

IDirect3DTexture9* TexturedObject::getTexture() {
    return pTexture;
}

void TexturedObject::render(const Transform *worldTransform) {
    d3dDevice->SetFVF(TEXTURE_VERTEX_FVF);
    d3dDevice->SetRenderState(D3DRS_LIGHTING, false);
    d3dDevice->SetTexture(0, pTexture);
    d3dDevice->SetTransform(D3DTS_WORLD, worldTransform->getTransformMatrix());

    d3dDevice->SetStreamSource(0, vbuffer, 0, sizeof(TEXTURE_VERTEX_WITH_NORMAL));
    d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangleCount);
}

void TexturedObject::setVertices(const std::vector<TEXTURE_VERTEX_WITH_NORMAL> vertices) {
    triangleCount = vertices.size() / 3;

    d3dDevice->CreateVertexBuffer(sizeof(TEXTURE_VERTEX_WITH_NORMAL)* vertices.size(),
                                 D3DUSAGE_WRITEONLY,
                                 TEXTURE_VERTEX_FVF,
                                 D3DPOOL_DEFAULT,
                                 &vbuffer,
                                 NULL);

    VOID* pVoid;

    vbuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices.data(), sizeof(TEXTURE_VERTEX_WITH_NORMAL)* vertices.size());
    vbuffer->Unlock();
}


// TexturedSquare
TexturedSquare::TexturedSquare(LPDIRECT3DDEVICE9 device, const D3DXVECTOR3& vect, const D3DXVECTOR3& shift, float size, std::vector<LPCWSTR> mipmapFilenames)
: TexturedObject(device) {

    std::vector<TEXTURE_VERTEX_WITH_NORMAL> vertices;
    D3DXVECTOR3 normal = vect;
    D3DXVec3Normalize(&normal, &normal);

    const int n = 100;
    const float min = -size / 2;
    const float max = size / 2;
    const float step = size / n;

    for (float u = min; u < max - step; u += step)
    {
        for (float v = min; v < max - step; v += step)
        {
            D3DXVECTOR3 u1, u2, u3, u4;
            if (normal.y != 0) {
                u1 = { u + shift.x, -(normal.x * u + normal.z * v) / normal.y + shift.y, v + shift.z };
                u2 = { u + step + shift.x, -(normal.x * (u + step) + normal.z * v) / normal.y + shift.y, v + shift.z };
                u3 = { u + shift.x, -(normal.x * u + normal.z * (v + step)) / normal.y + shift.y, v + step + shift.z };
                u4 = { u + step + shift.x, -(normal.x * (u + step) + normal.z * (v + step)) / normal.y + shift.y, v + step + shift.z };
            }
            else if (normal.z != 0) {
                u1 = { u + shift.x, v + shift.y, -(normal.x * u + normal.y * v) / normal.z + shift.z };
                u2 = { u + step + shift.x, v + shift.y, -(normal.x * (u + step) + normal.y * v) / normal.z + shift.z };
                u3 = { u + shift.x, v + step + shift.y, -(normal.x * u + normal.y * (v + step)) / normal.z + shift.z };
                u4 = { u + step + shift.x, v + step + shift.y, -(normal.x * (u + step) + normal.y * (v + step)) / normal.z + shift.z };
            }
            else if (normal.x != 0) {
                u1 = { -(normal.y * u + normal.z * v) / normal.x + shift.x, u + shift.y, v + shift.z };
                u2 = { -(normal.y * (u + step) + normal.z * v) / normal.x + shift.x, u + step + shift.y, v + shift.z };
                u3 = { -(normal.y * u + normal.z * (v + step)) / normal.x + shift.x, u + shift.y, v + step + shift.z };
                u4 = { -(normal.y * (u + step) + normal.z * (v + step)) / normal.x + shift.x, u + step + shift.y, v + step + shift.z };
            }

            TEXTURE_VERTEX_WITH_NORMAL v1 = { u1.x, u1.y, u1.z, normal, u / size + 0.5f, v / size + 0.5f};
            TEXTURE_VERTEX_WITH_NORMAL v2 = { u2.x, u2.y, u2.z, normal, (u + step) / size + 0.5f, v / size + 0.5f };
            TEXTURE_VERTEX_WITH_NORMAL v3 = { u3.x, u3.y, u3.z, normal, u / size + 0.5f, (v + step) / size + 0.5f };
            TEXTURE_VERTEX_WITH_NORMAL v4 = { u4.x, u4.y, u4.z, normal, (u + step) / size + 0.5f, (v + step) / size + 0.5f };

            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);

            vertices.push_back(v3);
            vertices.push_back(v2);
            vertices.push_back(v4);
        }
    }
    const UINT width = 1024;
    const UINT height = 1024;
    IDirect3DTexture9* prevTexture = NULL;

    D3DXCreateTexture(d3dDevice, width, height, mipmapFilenames.size(), 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->pTexture);
    D3DXCreateTexture(d3dDevice, width, height, mipmapFilenames.size(), 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &prevTexture);

    for (int i = 0; i < (int) mipmapFilenames.size(); ++i)
    {
        IDirect3DSurface9 *pSurf;
        prevTexture->GetSurfaceLevel(i, &pSurf);
        D3DXLoadSurfaceFromFile(pSurf, NULL, NULL, mipmapFilenames[i], NULL, D3DX_DEFAULT, 0, NULL);
        pSurf->Release();
    }

    d3dDevice->UpdateTexture(prevTexture, pTexture);
    prevTexture->Release();

    setVertices(vertices);
}