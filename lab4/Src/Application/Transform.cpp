#include "Transform.h"

void Transform::translate(float dx, float dy, float dz) {
    D3DXMATRIX t;
    D3DXMatrixTranslation(&t, dx, dy, dz);
    D3DXMatrixMultiply(&matrix, &matrix, &t);
}

void Transform::rotateX(float angle) {
    D3DXMATRIX rx;
    D3DXMatrixRotationX(&rx, angle);
    D3DXMatrixMultiply(&matrix, &matrix, &rx);
}

void Transform::rotateY(float angle) {
    D3DXMATRIX ry;
    D3DXMatrixRotationY(&ry, angle);
    D3DXMatrixMultiply(&matrix, &matrix, &ry);
}

void Transform::rotateZ(float angle) {
    D3DXMATRIX rz;
    D3DXMatrixRotationX(&rz, angle);
    D3DXMatrixMultiply(&matrix, &matrix, &rz);
}

void Transform::scale(float sx, float sy, float sz) {
    D3DXMATRIX s;
    D3DXMatrixScaling(&s, sx, sy, sz);
    D3DXMatrixMultiply(&matrix, &matrix, &s);
}


const D3DXMATRIX* Transform::getTransformMatrix() const {
    return &matrix;
}

Transform Transform::getInverse(const Transform &t) {
    Transform toRet;
    D3DXMatrixInverse(&toRet.matrix, NULL, &t.matrix);
    return toRet;
}

Transform::Transform(const Transform &obj) {
    matrix = obj.matrix;
}

void Transform::transform(const Transform &transform) {
    matrix *= *transform.getTransformMatrix();
}

void Transform::rotateAxis(float ax, float ay, float az, float phi) {
    D3DXMATRIX ra;
    D3DXVECTOR3 v = { ax, ay, az };
    D3DXMatrixRotationAxis(&ra, &v, phi);
    D3DXMatrixMultiply(&matrix, &matrix, &ra);
}
