#ifndef SPOT_LIGHT_H_INCLUDED__
#define SPIT_LIGHT_H_INCLUDED__

#include "LightSource.h"
#include <d3dx9.h>

class SpotLight : public LightSource {
private:
    static const D3DLIGHTTYPE TYPE = D3DLIGHT_SPOT;
public:
    SpotLight(D3DXVECTOR3 &pos,
              D3DXVECTOR3 &direction,
              float theta,
              float phi = 0,
              float range = 200,
              float fallOff = 1,
              float attenuation0 = 1,
              float attenuation1 = 0,
              float attenuation2 = 0,
              D3DXCOLOR &ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
              D3DXCOLOR &diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
              D3DXCOLOR &specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)) {

        light.Type = SpotLight::TYPE;
        light.Direction = direction;
        light.Phi = phi;
        light.Theta = theta;
        light.Range = range;
        light.Falloff = fallOff;
        light.Position = pos;
        light.Attenuation0 = attenuation0;
        light.Attenuation1 = attenuation1;
        light.Attenuation2 = attenuation2;
        light.Ambient = ambient;
        light.Diffuse = diffuse;
        light.Specular = specular;
    }

    void transform(D3DXMATRIX matrix) {
        D3DXVECTOR4 res;
        D3DXVec3Transform(&res, reinterpret_cast<D3DXVECTOR3 const *>(&light.Position), &matrix);
        light.Position = D3DXVECTOR3(res.x, res.y, res.z);

        D3DXVec3Transform(&res, reinterpret_cast<D3DXVECTOR3 const *>(&light.Direction), &matrix);
        light.Direction = D3DXVECTOR3(res.x, res.y, res.z);
    }

    void setPhi(float phi) {
        light.Phi = phi;
    }

    void setTheta(float theta) {
        light.Theta = theta;
    }

    void setPosition(D3DXVECTOR3 &pos) {
        light.Position = pos;
    }

    void setRange(float range) {
        light.Range = range;
    }

    void setFallOff(float fallOff) {
        light.Falloff = fallOff;
    }

    void setAttenuation0(float attenuation0) {
        light.Attenuation0 = attenuation0;
    }

    void setAttenuation1(float attenuation1) {
        light.Attenuation1 = attenuation1;
    }

    void setAttenuation2(float attenuation2) {
        light.Attenuation2 = attenuation2;
    }

    void setAmbient(D3DXCOLOR &ambient) {
        light.Ambient = ambient;
    }

    void setDiffuse(D3DXCOLOR &diffuse) {
        light.Diffuse = diffuse;
    }

    void setSpecular(D3DXCOLOR &specular) {
        light.Specular = specular;
    }

};

#endif