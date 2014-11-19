#include "Camera.h"


void Camera::flipNormalAndD3DCoord(D3DXVECTOR3 *p) {
    float buf = p->y;
    p->y = p->z;
    p->z = buf;
}

Camera::Camera() {
}

void Camera::setLookAt(D3DXVECTOR3 lookAt) {
    D3DXMatrixTranslation(&translateMatrix, -lookAt.x, -lookAt.y, -lookAt.z);
    pos += this->lookAt;
    pos -= lookAt;
}

void Camera::setPosition(D3DXVECTOR3 pos) {
    this->pos = pos;
    this->pos -= lookAt;

    flipNormalAndD3DCoord(&pos);
    radius = (float)sqrt((double)pos.x * pos.x + (double)pos.y * pos.y + (double)pos.z * pos.z);
    zenith = acosf(pos.z / radius);
    azimuth = atan2(pos.y, pos.x);
    flipNormalAndD3DCoord(&pos);
}

void Camera::setUpDirection(D3DXVECTOR3 upDirection) {
    this->upDirection = upDirection;
}

void Camera::rotateAzimuth(float phi) {
    azimuth += phi;
}

void Camera::rotateZenith(float phi) {
    zenith += phi;
}

void Camera::zoom(float shift) {
    radius += shift;
}

D3DXVECTOR3 Camera::getLookAt() {
    return lookAt;
}

D3DXVECTOR3 Camera::getPosition() {
    return pos;
}

D3DXVECTOR3 Camera::getUpDirection() {
    return upDirection;
}

const D3DMATRIX* Camera::getViewMatrix() {
    flipNormalAndD3DCoord(&pos);
    pos.x = radius * sin(zenith) * cos(azimuth);
    pos.y = radius * sin(zenith) * sin(azimuth);
    pos.z = radius * cos(zenith);
    flipNormalAndD3DCoord(&pos);

    D3DXMatrixLookAtLH(&viewMatrix, &pos, &D3DXVECTOR3(0, 0, 0), &upDirection);
    D3DXMatrixMultiply(&viewMatrix, &viewMatrix, &translateMatrix);

    return &viewMatrix;
}