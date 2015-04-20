#include "my_utils.h"

D3DXMATRIX WINAPI DXUTGetCubeMapViewMatrix(DWORD dwFace)
{
    D3DXVECTOR3 vEyePt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vLookDir;
    D3DXVECTOR3 vUpDir;

    switch (dwFace)
    {
    case D3DCUBEMAP_FACE_POSITIVE_X:
        vLookDir = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
        vUpDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        break;
    case D3DCUBEMAP_FACE_NEGATIVE_X:
        vLookDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
        vUpDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        break;
    case D3DCUBEMAP_FACE_POSITIVE_Y:
        vLookDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        vUpDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
        break;
    case D3DCUBEMAP_FACE_NEGATIVE_Y:
        vLookDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
        vUpDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
        break;
    case D3DCUBEMAP_FACE_POSITIVE_Z:
        vLookDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
        vUpDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        break;
    case D3DCUBEMAP_FACE_NEGATIVE_Z:
        vLookDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
        vUpDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        break;
    }

    // Set the view transform for this cubemap surface
    D3DXMATRIXA16 mView;
    D3DXMatrixLookAtLH(&mView, &vEyePt, &vLookDir, &vUpDir);
    return mView;
}
