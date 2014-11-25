#include "XMeshObject.h"

XMeshObject::XMeshObject() {
    this->d3dDevice = NULL;
    this->mesh = NULL;
}

void XMeshObject::setDevice(LPDIRECT3DDEVICE9 d3dDevice) {
    this->d3dDevice = d3dDevice;
}

void XMeshObject::loadModelFromFile(std::string name) {
    if (d3dDevice == NULL) {
        throw std::runtime_error("d3d device is null! Probably you does't call set(...) method!");
    }

    LPD3DXBUFFER materialBuffer;

    HRESULT hr = D3DXLoadMeshFromX(convertToWstring(name).c_str(), 0,
                                   d3dDevice, NULL,
                                   &materialBuffer, NULL, &materialNumber,
                                   &mesh);
    if (hr != S_OK) {
        throw std::runtime_error("error: cannot load mesh from .x file! hresult = " + hr);
    }

    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*) materialBuffer->GetBufferPointer();
    meshMaterials = new D3DMATERIAL9[materialNumber];
    meshTextures = new LPDIRECT3DTEXTURE9[materialNumber];

    for (unsigned int i = 0; i < materialNumber; i++)
    {
        // Copy the material
        meshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material
        meshMaterials[i].Ambient = meshMaterials[i].Diffuse;

        // Create the texture if it exists - it may not
        meshTextures[i] = NULL;
        if (d3dxMaterials[i].pTextureFilename)
            D3DXCreateTextureFromFile(d3dDevice, convertToWstring(d3dxMaterials[i].pTextureFilename).c_str(), &meshTextures[i]);
    }
    mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);
    materialBuffer->Release();
}

void XMeshObject::render() {
    if (d3dDevice == NULL)
        throw std::runtime_error("d3d device is null! Probably you does't call set(...) method!");


    for (size_t i = 0; i < lights.size(); ++i) {
        // changing lights positions and directions accordingly
        // to the full transformation from initial position
        lights[i]->getLightStruct()->Position = initialLightPositions[i];
        lights[i]->getLightStruct()->Direction = initialLightDirections[i];
        lights[i]->rotate(rotationMatrix);

        float dx = translationMatrix(3, 0) * scaleMatrix(0, 0);
        float dy = translationMatrix(3, 1) * scaleMatrix(1, 1);
        float dz = translationMatrix(3, 2) * scaleMatrix(2, 2);

        lights[i]->getLightStruct()->Position.x += dx;
        lights[i]->getLightStruct()->Position.y += dy;
        lights[i]->getLightStruct()->Position.z += dz;

        lights[i]->update();
    }

    for (unsigned int i = 0; i < materialNumber; i++)
    {
        // Set the material and texture for this subset
        d3dDevice->SetMaterial(&meshMaterials[i]);
        d3dDevice->SetTexture(0, meshTextures[i]);

        // Draw the mesh subset
        mesh->DrawSubset(i);
    }
}

void XMeshObject::addLight(LightSource *lightSource) {
    if (lightSource != NULL) {
        lights.push_back(lightSource);
        initialLightPositions.push_back(lightSource->getLightStruct()->Position);
        initialLightDirections.push_back(lightSource->getLightStruct()->Direction);
    }
}

XMeshObject::~XMeshObject() {
    if (mesh != NULL) {
        mesh->Release();
    }
    delete[] meshMaterials;
    delete[] meshTextures;
}