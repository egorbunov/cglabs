#include "AbstractRenderableObject.h"

AbstractRenderableObject::AbstractRenderableObject() {
    D3DXMatrixIdentity(&translationMatrix);
    D3DXMatrixIdentity(&scaleMatrix);
    D3DXMatrixIdentity(&rotationMatrix);
    D3DXMatrixIdentity(&worldMatrix);
}

void AbstractRenderableObject::translate(float dx, float dy, float dz) {
    D3DXMATRIX tmp;
    D3DXMatrixTranslation(&tmp, dx, dy, dz);
    D3DXMatrixMultiply(&translationMatrix, &translationMatrix, &tmp);
}

void AbstractRenderableObject::rotateX(float angle) {
    D3DXMATRIX tmp;
    D3DXMatrixRotationX(&tmp, angle);
    D3DXMatrixMultiply(&rotationMatrix, &rotationMatrix, &tmp);
}

void AbstractRenderableObject::rotateY(float angle) {
    D3DXMATRIX tmp;
    D3DXMatrixRotationY(&tmp, angle);
    D3DXMatrixMultiply(&rotationMatrix, &rotationMatrix, &tmp);
}

void AbstractRenderableObject::rotateZ(float angle) {
    D3DXMATRIX tmp;
    D3DXMatrixRotationZ(&tmp, angle);
    D3DXMatrixMultiply(&rotationMatrix, &rotationMatrix, &tmp);
}

void AbstractRenderableObject::scale(float sx, float sy, float sz) {
    D3DXMatrixScaling(&scaleMatrix, sx, sy, sz);
}

const D3DXMATRIX* AbstractRenderableObject::getWorldMatrix() {
    updateWorldMatrix();
    return &worldMatrix;
}

void AbstractRenderableObject::updateWorldMatrix() {
    D3DXMatrixMultiply(&worldMatrix, &rotationMatrix, &translationMatrix);
    D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleMatrix);
}