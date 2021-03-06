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
                             wasdCamera.rotate(((float)xPos / SCREEN_WIDTH) - ((float)m_nPrevMouseX / SCREEN_WIDTH),
                                               ((float)yPos / SCREEN_HEIGHT) - ((float)m_nPrevMouseY / SCREEN_HEIGHT));

                         }

                         m_nPrevMouseX = xPos;
                         m_nPrevMouseY = yPos;
                         break;
    }
    case WM_MOUSEWHEEL:
    {
                          if (!isWASDCameraActive) {
                              camera.zoom(-0.25f * (float)((signed short)(HIWORD(wParam))));
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

                       for (size_t i = 0; i < lights.size(); ++i) {
                           if (std::to_string(i + 1).at(0) == (char)wParam) {
                               lights[i]->changeState();
                           }
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
        m_d3ddev->SetTransform(D3DTS_VIEW, camera.getViewMatrix());
    }
    else {
        m_d3ddev->SetTransform(D3DTS_VIEW, wasdCamera.getViewMatrix());
    }

    
    circleIterator->next();
    car.rotateY(circleIterator->isClockwise() * circleIterator->getAngleStep());
    car.translate(circleIterator->getXTranslate(), 0, circleIterator->getZTranslate());
    
    m_d3ddev->SetFVF(CUSTOMFVF);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_d3ddev->SetTransform(D3DTS_WORLD, car.getWorldMatrix());
    car.render();

    m_d3ddev->SetMaterial(&globalMaterial);
    m_d3ddev->SetTransform(D3DTS_WORLD, plane.getWorldMatrix());
    plane.render();

    m_d3ddev->SetFVF(CartesianCoordinateSystem::FVF);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_d3ddev->SetTransform(D3DTS_WORLD, globalCoordSystem.getWorldMatrix());
    globalCoordSystem.render();
}

myApp::myApp(int nW, int nH, void* hInst, int nCmdShow) :
cglApp(nW, nH, hInst, nCmdShow)
, m_nPrevMouseX(-100)
, m_nPrevMouseY(-100)
, globalCoordSystem(worldCenter, 0xffffff)
, plane(D3DXVECTOR3(0.0f, 0.0f, 0.0f), { 0.0f, 1.0f, 0.0f }, 0, 110.0f, 0.0045f)
{
    for (int i = 0; i < MAX_KEYS; i++)
    {
        m_keysPressed[i] = false;
    }

    init_graphics();
}

void myApp::init_graphics() {
    m_d3ddev = m_pD3D->getDevice();
    m_d3ddev->SetRenderState(D3DRS_ZENABLE, true);
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, true);
    m_d3ddev->SetRenderState(D3DRS_SPECULARENABLE, true);
    m_d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    isWASDCameraActive = true;
    float initPos = 150.0f;
    wasdCamera.setPosition({ initPos, initPos, initPos });
    wasdCamera.setUpDirection({ 0.0f, 1.0f, 0.0f });
    wasdCamera.setLookAt(worldCenter);
    m_d3ddev->SetTransform(D3DTS_VIEW, wasdCamera.getViewMatrix());    // set the view transform to matView

    D3DXMatrixPerspectiveFovLH(&m_matProj,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               10.0f,    // the near view-plane
                               1000.0f);    // the far view-plane
    m_d3ddev->SetTransform(D3DTS_PROJECTION, &m_matProj);    // set the projection

    // one directed light and point light
    lights.push_back(new DirectedLight(
        D3DXVECTOR3(-1.0f, -1.0f, -1.0f),
        hexToColor(0),
        hexToColor(0x333300),
        hexToColor(0)));
    lights.push_back(new PointLight(
        D3DXVECTOR3(60.0f, 30.0f, -70.0f),
        100.0f,
        1.0f,
        0.000f,
        0.050f,
        0.001f,
        hexToColor(0xffffff),
        hexToColor(0xffffff),
        hexToColor(0xffffff)
        ));

    // headlights: A lot of magic numbers ;)
    D3DXCOLOR headlightColor = hexToColor(0xFFFF55);
    float headlightX = -12.5f;
    float headlightZ = -5.5f;
    float headlightY = -9.35f;
    lights.push_back(new SpotLight(
        D3DXVECTOR3(headlightX, headlightY, headlightZ),
        D3DXVECTOR3(-1.6f, 1.0f, 0.0f),
        D3DXToRadian(20.0f),
        D3DXToRadian(30.0f),
        70.0f,
        1.0f,
        0.0f,
        0.08f,
        0.0f,
        headlightColor,
        headlightColor,
        hexToColor(0)
        ));
    lights.push_back(new SpotLight(
        D3DXVECTOR3(headlightX, headlightY, headlightZ + 12.5f),
        D3DXVECTOR3(-1.6f, 1.0f, 0.0f),
        D3DXToRadian(20.0f),
        D3DXToRadian(30.0f),
        70.0f,
        1.0f,
        0.0f,
        0.08f,
        0.0f,
        headlightColor,
        headlightColor,
        hexToColor(0)
        ));


    car.setDevice(m_d3ddev);
    float carScaleFactor = 3;
    float radius = 25;
    car.loadModelFromFile("car00.x");
    car.translate(0, 0, radius);
    circleIterator = new CircleIterator(radius, 0.0f, 0.025f);

    car.addLight(lights[lights.size() - 1]);
    car.addLight(lights[lights.size() - 2]);

    car.rotateX(-D3DX_PI);
    car.rotateY(-D3DX_PI);

    car.scale(carScaleFactor, carScaleFactor, carScaleFactor);
    car.translate(0, 2.39f, 0);

    for (unsigned int i = 0; i < lights.size(); ++i) {
        lights[i]->create(m_d3ddev, i);
        lights[i]->switchOn();
    }

    ZeroMemory(&globalMaterial, sizeof(D3DMATERIAL9)); // clear out the struct for use
    globalMaterial.Diffuse = hexToColor(0);
    globalMaterial.Ambient = hexToColor(0xffffff);
    globalMaterial.Emissive = hexToColor(0x000010);
    globalMaterial.Specular = hexToColor(0);

    m_nClearColor = 0xFF111111;

    globalCoordSystem.scale(220.0f, 220.0f, 220.0f);
    globalCoordSystem.translate(-0.5, 0, -0.5);
    globalCoordSystem.create(m_d3ddev);

    plane.create(m_d3ddev);
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

    car.rotateZ(dy);
    car.rotateX(dx);
}

myApp::~myApp() {
    delete circleIterator;
    for (unsigned int i = 0; i < lights.size(); ++i) {
        delete lights[i];
    }
}