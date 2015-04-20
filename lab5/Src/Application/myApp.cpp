/**
@file     myApp.cpp
@brief    User's application class
@date     Created on 10/09/2005
@project  D3DBase
@author   Bvs
*/

// *******************************************************************
// includes

#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <zmouse.h>

#include "Library/cglD3D.h"
#include "myApp.h"

// *******************************************************************
// defines

namespace
{
    const float s_rMouseMove2Rotate = 0.005f;
    const float s_rMouseWheel2Zoom = 0.95f;
    const float s_rKbd2Rotate = 1.16f;
    const float s_rKbd2Zoom = 1.16f;
    const D3DXVECTOR3 worldCenter = { 0, 0, 0 };
}

// ******************************************************************
// static consts
const int myApp::MIPMAPS[] = { D3DTEXF_POINT, D3DTEXF_LINEAR, D3DTEXF_NONE };
const int myApp::MIN_MAG[2] = { D3DTEXF_POINT, D3DTEXF_LINEAR };

// *******************************************************************
// Methods

bool myApp::processInput(unsigned int nMsg, int wParam, long lParam)
{
    static bool isLButtonDown = false;
    switch (nMsg)
    {
    case WM_LBUTTONDOWN:
    {
                           isLButtonDown = true;
                           break;
    }
    case WM_LBUTTONUP: 
    {
                           isLButtonDown = false;
                           break;
    }
    case WM_CAPTURECHANGED:
    {
                              m_nPrevMouseX = m_nPrevMouseY = -100;
                              return 0;
    }
    case WM_NCMOUSEMOVE:
    {
                           m_nPrevMouseX = m_nPrevMouseY = -100;
                           break;
    }
    case WM_MOUSEMOVE:
    {
                         int xPos = GET_X_LPARAM(lParam);
                         int yPos = GET_Y_LPARAM(lParam);

                         if (isLButtonDown) {


                             if ((wParam & MK_LBUTTON) != 0 && m_nPrevMouseX >= 0 && !isWASDCameraActive) // 
                             {
                                 camera->rotateAzimuth(-s_rMouseMove2Rotate * (xPos - m_nPrevMouseX));
                                 camera->rotateZenith(-s_rMouseMove2Rotate * (yPos - m_nPrevMouseY));
                             }

                             if (isWASDCameraActive && m_nPrevMouseX >= 0) {
                                 wasdCamera->rotate(-((float)xPos / SCREEN_WIDTH) + ((float)m_nPrevMouseX / SCREEN_WIDTH),
                                                    -((float)yPos / SCREEN_HEIGHT) + ((float)m_nPrevMouseY / SCREEN_HEIGHT));

                             }
                         }
                         m_nPrevMouseX = xPos;
                         m_nPrevMouseY = yPos;
                         break;
    }
    case WM_MOUSEWHEEL:
    {
                          if (!isWASDCameraActive) {
                              camera->zoom(-0.25f * (float)((signed short)(HIWORD(wParam))));
                          }
                          break;
    }

    case WM_KEYDOWN:
    {
                       // Save flags for continuous controls
                       if (wParam < MAX_KEYS)
                           m_keysPressed[wParam] = true;

                       switch (wParam) {
                       case VK_SPACE:
                           if (isWASDCameraActive) {
                               camera->setPosition(wasdCamera->getPosition());
                               camera->setUpDirection(wasdCamera->getUpDirection());
                               camera->setLookAt(worldCenter);
                           }
                           else {
                               wasdCamera->setPosition(camera->getPosition());
                               wasdCamera->setUpDirection(camera->getUpDirection());
                               wasdCamera->setLookAt(worldCenter);
                           }
                           isWASDCameraActive = !isWASDCameraActive;
                           break;
                       case 'M':
                           mimpmapIndex = (mimpmapIndex + 1) % 3;
                           m_pD3D->getDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, MIPMAPS[mimpmapIndex]);
                           break;
                       }
                       break;
    }

    case WM_KEYUP:
    {
                     // Save flags for continuous controls
                     if (wParam < MAX_KEYS)
                         m_keysPressed[wParam] = false;
                     break;
    }
    }

    return cglApp::processInput(nMsg, wParam, lParam);
}

void myApp::renderInternal()
{
    static float u = 0;
    if (!isWASDCameraActive) {
        m_d3ddev->SetTransform(D3DTS_VIEW, camera->getViewMatrix());
    }
    else {
        m_d3ddev->SetTransform(D3DTS_VIEW, wasdCamera->getViewMatrix());
    }

    //reflectingCube->renderObjectsToCubeMap(aroundCubeObjects);

    //reflectingCube->render(reflectingCube->getWorldTransfrom());

    m_d3ddev->SetMaterial(&globalMaterial);
    for (RenderableObject *obj : aroundCubeObjects) {
        obj->render(obj->getWorldTransfrom());
    }
}

myApp::myApp(int nW, int nH, void* hInst, int nCmdShow) :
cglApp(nW, nH, hInst, nCmdShow)
, m_nPrevMouseX(-100)
, m_nPrevMouseY(-100)
{
    for (int i = 0; i < MAX_KEYS; i++)
    {
        m_keysPressed[i] = false;
    }

    init_graphics();
}

void myApp::init_graphics() {
    m_d3ddev = m_pD3D->getDevice();
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, false);
    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // creating cameras
    isWASDCameraActive = true;

    wasdCamera = new WASDCamera();
    camera = new Camera();

    float initPos = 250.0f;
    wasdCamera->setPosition({ initPos, 0.0f, initPos });
    wasdCamera->setUpDirection({ 0.0f, 1.0f, 0.0f });
    wasdCamera->setLookAt(worldCenter);
    m_d3ddev->SetTransform(D3DTS_VIEW, wasdCamera->getViewMatrix());    // set the view transform to matView

    D3DXMatrixPerspectiveFovLH(&m_matProj,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,    // the near view-plane
                               20000.0f);    // the far view-plane
    m_d3ddev->SetTransform(D3DTS_PROJECTION, &m_matProj);    // set the projection

    ZeroMemory(&globalMaterial, sizeof(D3DMATERIAL9)); // clear out the struct for use
    globalMaterial.Diffuse = hexToColor(0xffffff);
    globalMaterial.Ambient = hexToColor(0xffffff);
    globalMaterial.Emissive = hexToColor(0x000010);
    globalMaterial.Specular = hexToColor(0);
    

    m_nClearColor = 0xFF111111;


    SkyBox *box = new SkyBox(m_d3ddev);
    box->scale(10000.0f, 10000.0f, 10000.0f);
    objects.push_back(box);

    aroundCubeObjects.push_back(box);


    reflectingCube = new ReflectingCube(m_d3ddev);
    objects.push_back(reflectingCube);
    //reflectingCube->scale(100.0f, 100.0f, 100.0f);
}

namespace {

}

void myApp::update()
{
    // Call predecessor update
    cglApp::update();
    // Process keyboard
    float dx = 0.0f;
    float dy = 0.0f;
    float dr = 0.0f;
    if (m_keysPressed[VK_LEFT])
        dx -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_RIGHT])
        dx += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_UP])
        dy -= s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_DOWN])
        dy += s_rKbd2Rotate * m_timer.getDelta();
    if (m_keysPressed[VK_SUBTRACT])
        dr -= s_rKbd2Zoom * m_timer.getDelta();
    if (m_keysPressed[VK_ADD])
        dr += s_rKbd2Zoom * m_timer.getDelta();

    if (isWASDCameraActive) {
        if (m_keysPressed['A'])
            wasdCamera->moveLeft(CAMERA_SHIFT_STEP);
        if (m_keysPressed['D'])
            wasdCamera->moveRight(CAMERA_SHIFT_STEP);
        if (m_keysPressed['W'])
            wasdCamera->moveForward(CAMERA_SHIFT_STEP);
        if (m_keysPressed['S'])
            wasdCamera->moveBackward(CAMERA_SHIFT_STEP);
    }
}

myApp::~myApp() {    
    delete wasdCamera;
    delete camera;

    for (RenderableObject *o : objects)
        delete o;
}