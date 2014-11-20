#ifndef XMESH_OBJECT_H_INCLUDED__
#define XMESH_OBJECT_H_INCLUDED__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>

#include "TransformableObject.h"
#include "commonUtils.h"
#include "LightSource.h"

class XMeshObject : public TransformableObject {
private:
    LPD3DXMESH mesh;
    DWORD materialNumber;
    LPDIRECT3DDEVICE9 d3dDevice;

    D3DMATERIAL9 *meshMaterials;
    LPDIRECT3DTEXTURE9 *meshTextures;

    std::vector<LightSource*> lights;
public:
    XMeshObject();
    ~XMeshObject();
    void setDevice(LPDIRECT3DDEVICE9);
    void loadModelFromFile(std::string filename);
    void render();
    void addLight(LightSource *lightSource);
};

#endif