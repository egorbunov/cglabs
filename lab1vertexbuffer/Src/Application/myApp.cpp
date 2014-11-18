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
    const float s_rMouseWheel2Zoom = 0.005f;
    const float s_rKbd2Rotate = 1.16f;
    const float s_rKbd2Zoom = 1.16f;
}

// *******************************************************************
// Methods

bool myApp::processInput(unsigned int nMsg, int wParam, long lParam)
{
    switch (nMsg)
    {
    case WM_MOUSEMOVE:
    {
                         // Get mouse x & y (to the upper-left corner of the client area)
                         int xPos = GET_X_LPARAM(lParam);
                         int yPos = GET_Y_LPARAM(lParam);

                         if ((wParam & MK_LBUTTON) != 0 && m_nPrevMouseX >= 0) // 
                         {
                             camera.rotateAzimuth(-s_rMouseMove2Rotate * (xPos - m_nPrevMouseX));
                             camera.rotateZenith(-s_rMouseMove2Rotate * (yPos - m_nPrevMouseY));
                             m_d3ddev->SetTransform(D3DTS_VIEW, camera.getViewMatrix());
                             //rotateCamera(s_rMouseMove2Rotate*(xPos - m_nPrevMouseX),
                             //             s_rMouseMove2Rotate*(yPos - m_nPrevMouseY));
                         }

                         m_nPrevMouseX = xPos;
                         m_nPrevMouseY = yPos;
                         break;
    }
    case WM_MOUSEWHEEL:
    {
                          camera.zoom(-(float)((signed short)(HIWORD(wParam))));
                          m_d3ddev->SetTransform(D3DTS_VIEW, camera.getViewMatrix());
                          //zoom((int)((signed short)(HIWORD(wParam))));
                          break;
    }

    case WM_KEYDOWN:
    {
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
    m_d3ddev->SetTransform(D3DTS_WORLD, globalCoordSystem.getWorldMatrix());
    globalCoordSystem.render(m_d3ddev);
    m_d3ddev->SetTransform(D3DTS_WORLD, thorusCoordSystem.getWorldMatrix());
    thorusCoordSystem.render(m_d3ddev);
    m_d3ddev->SetTransform(D3DTS_WORLD, thorus.getWorldMatrix());
    thorus.render(m_d3ddev);
    m_d3ddev->SetFVF(CUSTOMFVF);
}

myApp::myApp(int nW, int nH, void* hInst, int nCmdShow)
: cglApp(nW, nH, hInst, nCmdShow)
, thorusCenter(100, 100, 100)
, m_nPrevMouseX(-100)
, m_nPrevMouseY(-100)
// my objects:
, camera({ 550.0f, 650.0f, 700.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f })
, thorus(100, 20, thorusCenter)
, globalCoordSystem({ 0, 0, 0 }, 0x00ff00)
, thorusCoordSystem(thorusCenter, 0xffd700, 150.0f)
{
    for (int i = 0; i < MAX_KEYS; i++)
    {
        m_keysPressed[i] = false;
    }

    init_graphics();

    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_nClearColor = 0xFF000000;

    thorus.create(m_d3ddev);
    globalCoordSystem.create(m_d3ddev);
    thorusCoordSystem.create(m_d3ddev);
}

void myApp::init_graphics() {
    m_d3ddev = m_pD3D->getDevice();
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


    thorus.rotateX(dx);
    thorus.rotateY(dy);
}