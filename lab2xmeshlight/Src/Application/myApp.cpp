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

// *******************************************************************
// Methods

bool myApp::processInput(unsigned int nMsg, int wParam, long lParam)
{
    switch (nMsg)
    {
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

                         if ((wParam & MK_LBUTTON) != 0 && m_nPrevMouseX >= 0 && !isWASDCameraActive) // 
                         {
                             camera.rotateAzimuth(-s_rMouseMove2Rotate * (xPos - m_nPrevMouseX));
                             camera.rotateZenith(-s_rMouseMove2Rotate * (yPos - m_nPrevMouseY));
                         }

                         if (isWASDCameraActive && m_nPrevMouseX >= 0) {
                             wasdCamera.rotate(((float)xPos / SCREEN_WIDTH) - ((float) m_nPrevMouseX / SCREEN_WIDTH),
                                               ((float)yPos / SCREEN_HEIGHT) - ((float)m_nPrevMouseY / SCREEN_HEIGHT));
                             
                         }

                         m_nPrevMouseX = xPos;
                         m_nPrevMouseY = yPos;
                         break;
    }
    case WM_MOUSEWHEEL:
    {
                          if (!isWASDCameraActive) {
                              camera.zoom(-(float)((signed short)(HIWORD(wParam))));
                          }
                          break;
    }

    case WM_KEYDOWN:
    {
                       if (wParam == VK_SPACE) {
                           if (isWASDCameraActive) {
                               camera.setPosition(wasdCamera.getPosition());
                               camera.setUpDirection(wasdCamera.getUpDirection());
                               camera.setLookAt(worldCenter);
                           }
                           else {
                               wasdCamera.setPosition(camera.getPosition());
                               wasdCamera.setUpDirection(camera.getUpDirection());
                               wasdCamera.setLookAt(worldCenter);
                           }
                           isWASDCameraActive = !isWASDCameraActive;
                           break;
                       }

                       // Save flags for continuous controls
                       if (wParam < MAX_KEYS)
                           m_keysPressed[wParam] = true;

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
    if (!isWASDCameraActive) {
        m_d3ddev->SetTransform(D3DTS_VIEW, camera.getViewMatrix());
    }
    else {
        m_d3ddev->SetTransform(D3DTS_VIEW, wasdCamera.getViewMatrix());
    }
    
    
    circleIterator->next();
    car.rotateY(circleIterator->isClockwise() * circleIterator->getAngleStep());
    car.translate(circleIterator->getXTranslate(), 0, circleIterator->getZTranslate());
    m_d3ddev->SetTransform(D3DTS_WORLD, car.getWorldMatrix());
    car.render();
    m_d3ddev->SetTransform(D3DTS_WORLD, globalCoordSystem.getWorldMatrix());
    globalCoordSystem.render();
    m_d3ddev->SetFVF(CUSTOMFVF);
}

myApp::myApp(int nW, int nH, void* hInst, int nCmdShow)
: cglApp(nW, nH, hInst, nCmdShow)
, m_nPrevMouseX(-100)
, m_nPrevMouseY(-100)
// my objects:
, globalCoordSystem(worldCenter, 0x00ff00)
{
    for (int i = 0; i < MAX_KEYS; i++)
    {
        m_keysPressed[i] = false;
    }

    isWASDCameraActive = true;
    init_graphics();

    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_nClearColor = 0xFF000000;

    globalCoordSystem.create(m_d3ddev);

    car.setDevice(m_d3ddev);
    float carScaleFactor = 15;
    float radius = 30;
    car.loadModelFromFile("car00.x");
    car.rotateX(D3DX_PI);
    car.rotateY(D3DX_PI);
    car.scale(carScaleFactor, carScaleFactor, carScaleFactor);
    car.translate(0, 2, radius);

    circleIterator = new CircleIterator(radius, 0, 0.025);
}

void myApp::init_graphics() {
    m_d3ddev = m_pD3D->getDevice();

    wasdCamera.setPosition({ 550.0f, 650.0f, 700.0f });
    wasdCamera.setUpDirection({ 0.0f, 1.0f, 0.0f });
    wasdCamera.setLookAt(worldCenter);

    camera.setPosition({ 550.0f, 650.0f, 700.0f });
    camera.setUpDirection({ 0.0f, 1.0f, 0.0f });
    camera.setLookAt(worldCenter);
    m_d3ddev->SetTransform(D3DTS_VIEW, camera.getViewMatrix());    // set the view transform to matView

    D3DXMatrixPerspectiveFovLH(&m_matProj,
        D3DXToRadian(45),    // the horizontal field of view
        (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
        100.0f,    // the near view-plane
        10000.0f);    // the far view-plane

    m_d3ddev->SetTransform(D3DTS_PROJECTION, &m_matProj);    // set the projection
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
            wasdCamera.moveLeft(CAMERA_SHIFT_STEP);
        if (m_keysPressed['D'])
            wasdCamera.moveRight(CAMERA_SHIFT_STEP);
        if (m_keysPressed['W'])
            wasdCamera.moveForward(CAMERA_SHIFT_STEP);
        if (m_keysPressed['S'])
            wasdCamera.moveBackward(CAMERA_SHIFT_STEP);
    }



    //car.rotateZ(dy);
    //car.rotateX(dx);
}

myApp::~myApp() {
    delete circleIterator;
}