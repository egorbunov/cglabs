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

static std::vector<LPCWSTR> getMipMapFilenames() {
    std::vector<LPCWSTR> res;
    res.push_back(L".\\Resources\\mipmap32.png");
    res.push_back(L".\\Resources\\mipmap64.png");
    res.push_back(L".\\Resources\\mipmap128.png");
    res.push_back(L".\\Resources\\mipmap256.png");
    return res;
}
const std::vector<LPCWSTR> myApp::MIPMAP_FILENAMES = getMipMapFilenames();


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
                       case 'M':
                           mimpmapIndex = (mimpmapIndex + 1) % 3;
                           m_pD3D->getDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, MIPMAPS[mimpmapIndex]);
                           break;
                       case 'F':
                           minIndex = (minIndex + 1) % 2;
                           m_pD3D->getDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, MIN_MAG[minIndex]);
                           break;
                       case 'G':
                           magIndex = (magIndex + 1) % 2;
                           m_pD3D->getDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, MIN_MAG[magIndex]);
                           break;
                       }

                       // processing numeric key press (switch off/on lights)
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
    

    m_d3ddev->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *((DWORD*) &mipMapBias));
    for (int i = 0; i < (int) texturedObjects.size(); ++i) {
        texturedObjects[i]->prepare();
        texturedObjects[i]->render();
    }


    airplane.render();

    m_d3ddev->SetMaterial(&globalMaterial);
    plane.render();
    globalCoordSystem.render();
}

myApp::myApp(int nW, int nH, void* hInst, int nCmdShow) :
cglApp(nW, nH, hInst, nCmdShow)
, m_nPrevMouseX(-100)
, m_nPrevMouseY(-100)
, mimpmapIndex(0)
, minIndex(0)
, magIndex(0)
, mipMapBias(0.0f)
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
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, true);
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
        hexToColor(0x403020),
        hexToColor(0xffffff),
        hexToColor(0)));
    lights.push_back(new PointLight(
        D3DXVECTOR3(60.0f, 30.0f, -70.0f),
        400.0f,
        1.0f,
        0.000f,
        0.050f,
        0.001f,
        hexToColor(0xffffff),
        hexToColor(0xffffff),
        hexToColor(0xffffff)
        ));
    lights.push_back(new PointLight(
        D3DXVECTOR3(-60.0f, 30.0f, 70.0f),
        400.0f,
        1.0f,
        0.000f,
        0.050f,
        0.001f,
        hexToColor(0xffffff),
        hexToColor(0xffffff),
        hexToColor(0xffffff)
        ));

    // headlights: A lot of magic numbers ;)
    D3DXCOLOR headlightColor = hexToColor(0xF0F9F0);
    float headlightX = -5.0f;
    float headlightZ = 9.5f;
    float headlightY = 6.35f;

    lights.push_back(new SpotLight(
        D3DXVECTOR3(headlightX, headlightY, headlightZ),
        D3DXVECTOR3(0.0f, -1.0f, 0.9f),
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
        D3DXVECTOR3(headlightX + 5.0f, headlightY, headlightZ),
        D3DXVECTOR3(0.0f, -1.0f, 0.9f),
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

    airplane.setDevice(m_d3ddev);
    float carScaleFactor = 5;
    float radius = 15;
    airplane.loadModelFromFile(".\\Resources\\airplane00.x");
    airplane.translate(0, 0, radius);
    circleIterator = new CircleIterator(radius, 0.0f, 0.007f);

    airplane.addLight(lights[lights.size() - 1]);
    airplane.addLight(lights[lights.size() - 2]);

    airplane.rotateY(D3DX_PI / 2);
    //car.rotateZ(D3DX_PI / 2);


    airplane.scale(carScaleFactor, carScaleFactor, carScaleFactor);
    airplane.translate(0, 2.39f, 0);

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

    // textured objects creation

    float scale = 2.5f;
    texturedObjects.push_back(new TexturedSquare(m_d3ddev, { 0.0f, 1.0f, 0.0f }, { 0.0f, 30.0f, 0.0f }, 10.0f, MIPMAP_FILENAMES));
    texturedObjects.push_back(new TexturedSquare(m_d3ddev, { 0.0f, -1.0f, 0.0f }, { 0.0f, 20.0f, 0.0f }, 10.0f, MIPMAP_FILENAMES));
    texturedObjects.push_back(new TexturedSquare(m_d3ddev, { 1.0f, 0.0f, 0.0f }, { 5.0f, 25.0f, 0.0f }, 10.0f, MIPMAP_FILENAMES));
    texturedObjects.push_back(new TexturedSquare(m_d3ddev, { 1.0f, 0.0f, 0.0f }, { -5.0f, 25.0f, 0.0f }, 10.0f, MIPMAP_FILENAMES));
    texturedObjects.push_back(new TexturedSquare(m_d3ddev, { 0.0f, 0.0f, 1.0f }, { 0.0f, 25.0f, 5.0f }, 10.0f, MIPMAP_FILENAMES));
    texturedObjects.push_back(new TexturedSquare(m_d3ddev, { 0.0f, 0.0f, -1.0f }, { 0.0f, 25.0f, -5.0f }, 10.0f, MIPMAP_FILENAMES));
    for (int i = 0; i < (int) texturedObjects.size(); ++i) {
        texturedObjects[i]->scale(scale, scale, scale);
        texturedObjects[i]->translate(0, -10.0f, 0);
    }
}

void myApp::doTransformations() {

    circleIterator->next();
    airplane.rotateY(circleIterator->isClockwise() * circleIterator->getAngleStep());
    airplane.translate(circleIterator->getXTranslate(), 0, circleIterator->getZTranslate());
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

    if (m_keysPressed[VK_SHIFT]) {
        if (m_keysPressed[VK_ADD] || m_keysPressed[VK_OEM_PLUS]) {
            mipMapBias += 0.2f;
            m_keysPressed[VK_OEM_PLUS] = false;
            m_keysPressed[VK_ADD] = false;
        }
        if (m_keysPressed[VK_SUBTRACT] || m_keysPressed[VK_OEM_MINUS]) {
            mipMapBias -= 0.2f;
            m_keysPressed[VK_OEM_MINUS] = false;
            m_keysPressed[VK_SUBTRACT] = false;
        }
    }

    doTransformations();

}

myApp::~myApp() {
    delete circleIterator;
    for (unsigned int i = 0; i < lights.size(); ++i) {
        delete lights[i];
    }
}