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

    virtual void create(LPDIRECT3DDEVICE9 device, unsigned int index){
        this->device = device;
        this->index = index;
        device->SetLight(index, &light);
    }

    virtual void switchOn() {
        if (device == NULL)
            throw std::runtime_error("device is null! Probably you does't call create(...) method!");
        device->LightEnable(index, isOn = true);
    }

    virtual void switchOff() {
        if (device == NULL)
            throw std::runtime_error("device is null! Probably you does't call create(...) method!");
        device->LightEnable(index, isOn = false);
    }

    virtual void changeState() {
        if (device == NULL)
            throw std::runtime_error("device is null! Probably you does't call create(...) method!");
        device->LightEnable(index, isOn = !isOn);
    }

    virtual void transform(D3DXMATRIX &transfrom) {

    }
};

#endif