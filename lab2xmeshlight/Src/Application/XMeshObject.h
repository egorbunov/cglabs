#ifndef XMESH_OBJECT_H_INCLUDED__
#define XMESH_OBJECT_H_INCLUDED__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#include "AbstractRenderableObject.h"
#include "commonUtils.h"

class XMeshObject : public AbstractRenderableObject {
private:
    LPD3DXMESH mesh;
    DWORD materialNumber;
    LPDIRECT3DDEVICE9 d3dDevice;

    D3DMATERIAL9 *meshMaterials;
    LPDIRECT3DTEXTURE9 *meshTextures;
public:
    XMeshObject();
    ~XMeshObject();
    void setDevice(LPDIRECT3DDEVICE9);
    void loadModelFromFile(std::string filename);
    void render() override;
};

#endif