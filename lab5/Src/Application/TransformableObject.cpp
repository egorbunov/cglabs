#include "TransformableObject.h"

TransformableObject::TransformableObject() {
}

const D3DXMATRIX* TransformableObject::getTransformMatrix() {
    return worldTransform.getTransformMatrix();
}



void TransformableObject::rotateX(float phi) {
    worldTransform.rotateX(phi);
}

void TransformableObject::rotateY(float phi) {
    worldTransform.rotateY(phi);
}

void TransformableObject::rotateZ(float phi) {
    worldTransform.rotateZ(phi);
}

void TransformableObject::transform(Transform &transform) {
    worldTransform.transform(transform);
}


void TransformableObject::translate(float dx, float dy, float dz) {
    worldTransform.translate(dx, dy, dz);
}


void TransformableObject::scale(float sx, float sy, float sz) {
    worldTransform.scale(sx, sy, sz);
}

const Transform *TransformableObject::getWorldTransfrom() {
    return &worldTransform;
}

void TransformableObject::rotateAxis(float ax, float ay, float az, float phi) {
    worldTransform.rotateAxis(ax, ay, az, phi);
}

void TransformableObject::rotateAxis(D3DXVECTOR3 &axis, float phi) {
    worldTransform.rotateAxis(axis.x, axis.y, axis.z, phi);
}