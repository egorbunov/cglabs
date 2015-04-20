#include "WASDCamera.h"

WASDCamera::WASDCamera() {
    D3DXMatrixIdentity(&translateMatrix);
    D3DXMatrixIdentity(&rotationMatrix);
    D3DXMatrixIdentity(&viewMatrix);
}

void WASDCamera::setUpDirection(D3DXVECTOR3 upDirection) {
    this->upDirection = upDirection;
}

void WASDCamera::setLookAt(D3DXVECTOR3 lookAt) {
    this->lookAt = lookAt;
}

void WASDCamera::setPosition(D3DXVECTOR3 pos) {
    this->pos = pos;
}

void WASDCamera::rotate(float dx, float dy) {
    D3DXVECTOR3 leftDirection;
    D3DXVECTOR3 lookAtDirection = lookAt - pos;
    D3DXVec3Cross(&leftDirection, &upDirection, &lookAtDirection);

    D3DXMatrixRotationAxis(&rotationMatrix, &leftDirection, dy);
    D3DXVECTOR4 newLookAtDirection;
    D3DXVec3Transform(&newLookAtDirection, &lookAtDirection, &rotationMatrix);

    D3DXMatrixRotationAxis(&rotationMatrix, &upDirection, dx);
    D3DXVec4Transform(&newLookAtDirection, &newLookAtDirection, &rotationMatrix);

    

    lookAt.x = pos.x + newLookAtDirection.x;
    lookAt.y = pos.y + newLookAtDirection.y;
    lookAt.z = pos.z + newLookAtDirection.z;
}

void WASDCamera::moveLeft(float shift) {
    D3DXVECTOR3 leftDirection;
    D3DXVECTOR3 lookAtDirection = lookAt - pos;
    D3DXVec3Cross(&leftDirection, &lookAtDirection, &upDirection);

    D3DXVec3Normalize(&leftDirection, &leftDirection);
    pos = pos + leftDirection * shift;
    lookAt = lookAt + leftDirection * shift;

}

void WASDCamera::moveRight(float shift) {
    D3DXVECTOR3 rightDirection;
    D3DXVECTOR3 lookAtDirection = lookAt - pos;
    D3DXVec3Cross(&rightDirection, &upDirection, &lookAtDirection);

    D3DXVec3Normalize(&rightDirection, &rightDirection);
    pos = pos + rightDirection * shift;
    lookAt = lookAt + rightDirection * shift;
}

void WASDCamera::moveForward(float shift) {
    D3DXVECTOR3 lookAtDirection = lookAt - pos;
    D3DXVec3Normalize(&lookAtDirection, &lookAtDirection);
    pos = pos + lookAtDirection * shift;
    lookAt = lookAt + lookAtDirection * shift;
}

void WASDCamera::moveBackward(float shift) {
    D3DXVECTOR3 lookAtDirection = lookAt - pos;
    D3DXVec3Normalize(&lookAtDirection, &lookAtDirection);
    pos = pos - lookAtDirection * shift;
    lookAt = lookAt - lookAtDirection * shift;
}

const D3DMATRIX* WASDCamera::getViewMatrix() {
    D3DXMatrixLookAtLH(&viewMatrix, &pos, &lookAt , &upDirection);
    return &viewMatrix;
}

D3DXVECTOR3 WASDCamera::getPosition() const {
    return pos;
}

D3DXVECTOR3 WASDCamera::getUpDirection() const {
    return upDirection;
}

D3DXVECTOR3 WASDCamera::getLookAt() const {
    return lookAt;
}