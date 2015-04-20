#ifndef CUBE_H_INCLUDED__
#define CUBE_H_INCLUDED__

#include <vector>
#include "RenderableObject.h"
#include "texture_objects.h"
#include "my_utils.h"

class ReflectingCube : public RenderableObject {
    static const std::vector<LPCWSTR> MIPMAP_FILENAMES;
    LPDIRECT3DDEVICE9 device;

    LPD3DXEFFECT effect;
    IDirect3DVertexDeclaration9 *m_vertexDeclaration;
    IDirect3DCubeTexture9* cubeEnvTexture;
    std::vector<TexturedSquare*> faces;
public:
    ReflectingCube(LPDIRECT3DDEVICE9 d3dDevice);
    ~ReflectingCube();
    void render(const Transform *worldTransform);

    void renderObjectsToCubeMap(std::vector<RenderableObject*> objects);
};


#endif