#ifndef CUBE_H_INCLUDED__
#define CUBE_H_INCLUDED__

#include <vector>
#include "RenderableObject.h"
#include "texture_objects.h"
#include "my_utils.h"

class ReflectingCube : public RenderableObject {
    static const std::vector<LPCWSTR> FACE_1_TEXS;
    static const std::vector<LPCWSTR> FACE_2_TEXS;
    static const std::vector<LPCWSTR> FACE_3_TEXS;
    static const std::vector<LPCWSTR> FACE_4_TEXS;
    static const std::vector<LPCWSTR> FACE_5_TEXS;
    static const std::vector<LPCWSTR> FACE_6_TEXS;


    LPDIRECT3DDEVICE9 device;

    float fresnelPow;

    LPD3DXEFFECT effect;
    IDirect3DVertexDeclaration9 *m_vertexDeclaration;
    IDirect3DCubeTexture9* cubeEnvTexture;
    std::vector<TexturedSquare*> faces;

    const D3DMATRIX* curViewMat;
public:
    ReflectingCube(LPDIRECT3DDEVICE9 d3dDevice);
    ~ReflectingCube();
    void render(const Transform *worldTransform);

    void renderObjectsToCubeMap(std::vector<RenderableObject*> objects);

    void incFresnelPow();
    void decFresnelPow();
};


#endif