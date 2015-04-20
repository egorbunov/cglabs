#include "Camera.h"


void Camera::flipNormalAndD3DCoord(D3DXVECTOR3 *p) {
    float buf = p->y;
    p->y = p->z;
    p->z = buf;
}

Camera::Camera() {
    needToChangeDirection = false;
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
    if (azimuth >= 2 * D3DX_PI)
        azimuth -= 2 * D3DX_PI;
    if (azimuth <= -2 * D3DX_PI)
        azimuth += 2 * D3DX_PI;
}

void Camera::rotateZenith(float phi) {
    zenith += phi;
    if (zenith >= 2 * D3DX_PI)
        zenith -= 2 * D3DX_PI;
    if (zenith <= -2 * D3DX_PI)
        zenith += 2 * D3DX_PI;

    needToChangeDirection = false;
    if ((zenith >= 0 && zenith - phi <= 0) || (zenith <= 0 && zenith - phi >= 0)
        || (zenith >= D3DX_PI && zenith - phi <= D3DX_PI) || (zenith <= D3DX_PI && zenith - phi >= D3DX_PI)
        || (zenith <= -D3DX_PI && zenith - phi >= -D3DX_PI) || (zenith >= -D3DX_PI && zenith - phi <= -D3DX_PI)) {
        needToChangeDirection = true;
    }
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

    if (needToChangeDirection) {
        upDirection = -upDirection;
        needToChangeDirection = false;
    }
    D3DXMatrixLookAtLH(&viewMatrix, &pos, &D3DXVECTOR3(0, 0, 0), &upDirection);
    D3DXMatrixMultiply(&viewMatrix, &viewMatrix, &translateMatrix);

    return &viewMatrix;
}