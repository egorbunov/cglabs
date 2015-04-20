#ifndef XMESH_OBJECT_H_INCLUDED__
#define XMESH_OBJECT_H_INCLUDED__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>

#include "RenderableObject.h"
#include "commonUtils.h"

class XMeshObject : public RenderableObject {
private:
    LPD3DXMESH mesh;
    DWORD materialNumber;
    LPDIRECT3DDEVICE9 d3dDevice;

    D3DMATERIAL9 *meshMaterials;
    LPDIRECT3DTEXTURE9 *meshTextures;

    std::vector<D3DVECTOR> initialLightPositions;
    std::vector<D3DVECTOR> initialLightDirections;
public:
    XMeshObject(LPDIRECT3DDEVICE9);
    ~XMeshObject();
    void loadModelFromFile(std::string filename);
    void render(const Transform *worldTransform);
};

#endif