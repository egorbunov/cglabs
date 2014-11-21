#ifndef DIRECTED_LIGHT_H_INCLUDED__
#define DIRECTED_LIGHT_H_INCLUDED__

#include "LightSource.h"
#include <d3dx9.h>

class DirectedLight : public LightSource {
private:
    static const D3DLIGHTTYPE TYPE = D3DLIGHT_DIRECTIONAL;
public:
    DirectedLight(D3DXVECTOR3 direction, 
                  D3DXCOLOR ambient,
                  D3DXCOLOR diffuse,
                  D3DXCOLOR specular) {
        light.Ambient = ambient;
        light.Specular = specular;
        light.Diffuse = diffuse;
        light.Direction = direction;

        light.Type = DirectedLight::TYPE;
    }

    void setDirection(D3DXVECTOR3 direction) {
        light.Direction = direction;
    }

    void setAmbient(D3DXCOLOR ambient) {
        light.Ambient = ambient;
    }

    void setDiffuse(D3DXCOLOR diffuse) {
        light.Diffuse = diffuse;
    }

    void setSpecular(D3DXCOLOR specular) {
        light.Specular = specular;
    }
};

#endif