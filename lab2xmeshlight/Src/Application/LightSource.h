#ifndef LIGHT_SOURCE_H_INCLUDED__
#define LIGHT_SOURCE_H_INCLUDED__

#include <d3d9.h>
#include <stdexcept>

class LightSource {
protected:
    D3DLIGHT9 light;
    bool isOn;
    unsigned int index;
    LPDIRECT3DDEVICE9 device;
public:
    LightSource() : isOn(false), device(NULL) {
        ZeroMemory(&light, sizeof(light));
    }
    virtual ~LightSource() = 0 {}

    virtual unsigned getIndex() {
        return index;
    }

    virtual D3DLIGHT9* getLightStruct() {
        return &light;
    }

    virtual void create(LPDIRECT3DDEVICE9 device, unsigned int index){
        this->device = device;
        this->index = index;
        device->SetLight(index, &light);
    }

    virtual void update() {
        if (isOn)
            device->SetLight(index, &light);
    }

    virtual void switchOn() {
        if (device == NULL)
            throw std::runtime_error("device is null! Probably you does't call create(...) method!");
        isOn = true;
        device->LightEnable(index, isOn);
    }

    virtual void switchOff() {
        if (device == NULL)
            throw std::runtime_error("device is null! Probably you does't call create(...) method!");
        isOn = false;
        device->LightEnable(index, isOn);
    }

    virtual void changeState() {
        if (device == NULL)
            throw std::runtime_error("device is null! Probably you does't call create(...) method!");
        isOn = !isOn;
        device->LightEnable(index, isOn);
    }

    virtual void translate(float dx, float dy, float dz) {
        light.Position.x += dx;
        light.Position.y += dy;
        light.Position.z += dz;
    }

    virtual void rotate(D3DXMATRIX &rotationMatrix) {
        D3DXVECTOR4 res;
        D3DXVec3Transform(&res, reinterpret_cast<D3DXVECTOR3 const *>(&light.Direction), &rotationMatrix);
        light.Direction = D3DXVECTOR3(res.x, res.y, res.z);
        D3DXVec3Transform(&res, reinterpret_cast<D3DXVECTOR3 const *>(&light.Position), &rotationMatrix);
        light.Position = D3DXVECTOR3(res.x, res.y, res.z);
    }

    virtual void scale(float sx, float sy, float sz) {
        light.Position.x *= sx;
        light.Position.y *= sy;
        light.Position.z *= sz;
    }
};

#endif