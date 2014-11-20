#include "TransformableObject.h"

TransformableObject::TransformableObject() {
    D3DXMatrixIdentity(&translationMatrix);
    D3DXMatrixIdentity(&scaleMatrix);
    D3DXMatrixIdentity(&rotationMatrix);
    D3DXMatrixIdentity(&worldMatrix);
}

TransformableObject* TransformableObject::translate(float dx, float dy, float dz) {
    D3DXMATRIX tmp;
    D3DXMatrixTranslation(&tmp, dx, dy, dz);
    D3DXMatrixMultiply(&translationMatrix, &translationMatrix, &tmp);
    return this;
}

TransformableObject* TransformableObject::rotateX(float angle) {
    D3DXMATRIX tmp;
    D3DXMatrixRotationX(&tmp, angle);
    D3DXMatrixMultiply(&rotationMatrix, &rotationMatrix, &tmp);
    return this;
}

TransformableObject* TransformableObject::rotateY(float angle) {
    D3DXMATRIX tmp;
    D3DXMatrixRotationY(&tmp, angle);
    D3DXMatrixMultiply(&rotationMatrix, &rotationMatrix, &tmp);
    return this;
}

TransformableObject* TransformableObject::rotateZ(float angle) {
    D3DXMATRIX tmp;
    D3DXMatrixRotationZ(&tmp, angle);
    D3DXMatrixMultiply(&rotationMatrix, &rotationMatrix, &tmp);
    return this;
}

TransformableObject* TransformableObject::scale(float sx, float sy, float sz) {
    D3DXMatrixScaling(&scaleMatrix, sx, sy, sz);
    return this;
}

const D3DXMATRIX* TransformableObject::getWorldMatrix() {
    updateWorldMatrix();
    return &worldMatrix;
}

void TransformableObject::updateWorldMatrix() {
    D3DXMatrixMultiply(&worldMatrix, &rotationMatrix, &translationMatrix);
    D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleMatrix);
}