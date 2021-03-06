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
    doTransformations();

    static float u = 0;
    if (!isWASDCameraActive) {
        m_d3ddev->SetTransform(D3DTS_VIEW, camera->getViewMatrix());
    }
    else {
        m_d3ddev->SetTransform(D3DTS_VIEW, wasdCamera->getViewMatrix());
    }
    
    
    m_d3ddev->SetMaterial(&globalMaterial);
    for (RenderableObject *obj : objects) {
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


void myApp::prepareLights() {
    // one directed light and point light
    lights.push_back(new DirectedLight(
        D3DXVECTOR3(-1.0f, -1.0f, -1.0f),
        hexToColor(0x403020),
        hexToColor(0xffffff),
        hexToColor(0)));
    lights.push_back(new PointLight(
        D3DXVECTOR3(60.0f, 40.0f, -70.0f),
        300.0f,
        4.0f,
        0.000f,
        0.050f,
        0.001f,
        hexToColor(0xCCFF66),
        hexToColor(0xffffff),
        hexToColor(0xffffff)
        ));
    lights.push_back(new PointLight(
        D3DXVECTOR3(-60.0f, 30.0f, 70.0f),
        200.0f,
        1.0f,
        0.000f,
        0.050f,
        0.001f,
        hexToColor(0xffffff),
        hexToColor(0xffffff),
        hexToColor(0xffffff)
        ));
    lights.push_back(new PointLight(
        D3DXVECTOR3(60.0f, 30.0f, 70.0f),
        200.0f,
        1.0f,
        0.000f,
        0.050f,
        0.001f,
        hexToColor(0xffffff),
        hexToColor(0xffffff),
        hexToColor(0xffffff)
        ));
    lights.push_back(new PointLight(
        D3DXVECTOR3(-60.0f, 30.0f, -70.0f),
        200.0f,
        1.0f,
        0.000f,
        0.050f,
        0.001f,
        hexToColor(0xffffff),
        hexToColor(0xffffff),
        hexToColor(0xffffff)
        ));

    for (unsigned int i = 0; i < lights.size(); ++i) {
        lights[i]->create(m_d3ddev, i);
        lights[i]->switchOn();
    }

}

void myApp::init_graphics() {
    m_d3ddev = m_pD3D->getDevice();
    m_d3ddev->SetRenderState(D3DRS_LIGHTING, true);
    m_d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
    m_d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

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
                               2000.0f);    // the far view-plane
    m_d3ddev->SetTransform(D3DTS_PROJECTION, &m_matProj);    // set the projection

    prepareLights();
    
    ZeroMemory(&globalMaterial, sizeof(D3DMATERIAL9)); // clear out the struct for use
    globalMaterial.Diffuse = hexToColor(0xffffff);
    globalMaterial.Ambient = hexToColor(0xffffff);
    globalMaterial.Emissive = hexToColor(0x000010);
    globalMaterial.Specular = hexToColor(0);
    

    m_nClearColor = 0xFF111111;

    // creating plane
    Plane *plane = new Plane(m_d3ddev, 0, 0.005f);
    plane->scale(100.0f, 100.0f, 100.0f);
    plane->translate(0.0f, -80.0f, 0.0f);
    objects.push_back(plane);

    //objects.push_back(new Cylinder(m_d3ddev, D3DXVECTOR3(0.0f, 0.0f, 0.0f), { D3DX_PI / 2, 0.0f, 0.0f }, 25, 50, hexToColor(0xC91B12)));

    prepareSkeleton();

}

void myApp::doTransformations() {
    for (RandomRotation *rr : randomRotations)
        rr->next();
}

namespace {

}


void myApp::prepareSkeleton() {
    float ropeRad = 0.25f;
    float bellRad = 3.5f;
    float crossRad = 4.0f;
    float crossHeight = 50.0f;
    float bellHeight = 27.0f;
    float ropeHeight = 23.0f;
    float ropeHeight2 = 15.0f;
    float rootRopeHeight = 70.f;
    float ropeInBellH = 2.5f;

    D3DXCOLOR crossColor = hexToColor(0x59390C);
    D3DXCOLOR ropeColor = hexToColor(0x5C5485);


    Cylinder *rootRope = new Cylinder(m_d3ddev, ropeRad, rootRopeHeight, hexToColor(0x616161));

    float height = 100.0f;

    // initializing root of the structure and 2 crossing cylinders
    mSkeleton = new Skeleton(rootRope);
    rootRope->rotateX(D3DX_PI / 2);
    rootRope->translate(0, -rootRopeHeight / 2, 0);
    //mSkeleton->rotateY(D3DX_PI / 4);
    mSkeleton->translate(0, height, 0);

    
    Cylinder *crossA = new Cylinder(m_d3ddev, crossRad, crossHeight, crossColor);
    Skeleton *nodeCrossA = new Skeleton(crossA);
    crossA->translate(0, rootRopeHeight / 2.0f, 0);
    crossA->rotateX(D3DX_PI / 2);
    mSkeleton->addChild(nodeCrossA);

    
    Cylinder *crossB = new Cylinder(m_d3ddev, crossRad, crossHeight, crossColor);
    Skeleton *nodeCrossB = new Skeleton(crossB);
    crossB->rotateY(D3DX_PI / 2.0);
    nodeCrossA->addChild(nodeCrossB);

    
    // adding bells and ropes
    auto addToCross = [&](Skeleton *p,  D3DVECTOR transl) {
        Cylinder *rope = new Cylinder(m_d3ddev, ropeRad, ropeHeight, ropeColor);
        rope->rotateX(D3DX_PI / 2);
        rope->translate(transl.x, transl.y, transl.z);
        Skeleton *nodeRope = new Skeleton(rope);
        p->addChild(nodeRope);
        Cylinder *bell = new Cylinder(m_d3ddev, bellRad, bellHeight, getRandomColor());
        bell->rotateX(-D3DX_PI / 2);
        bell->translate(0, -(ropeHeight / 2 + bellHeight / 2) + ropeInBellH, 0);
        bell->rotateX(D3DX_PI / 2);
        Skeleton * nodeBell = new Skeleton(bell);
        nodeRope->addChild(nodeBell);
        

        
        RandomRotation *rr = new RandomRotation(-0.2f, 0.2f, 0.002f, 0.003f);
        rr->addTransformation(rope, [ropeHeight, bellRad](TransformableObject* o, float val) -> void {
            o->rotateX(val);

        });
        randomRotations.push_back(rr);
        
        rr = new RandomRotation(-D3DX_PI, D3DX_PI, 0.002f, 0.003f);
        rr->addTransformation(rope, [ropeHeight, transl](TransformableObject* o, float val) -> void {
            o->translate(-transl.x, 0.0f, 0.0f);
            o->rotateY(val);
            o->translate(transl.x, 0.0f, 0.0f);
        });
        randomRotations.push_back(rr);

        
        rr = new RandomRotation(-0.05f, 0.05f, 0.002f, 0.004f);
        rr->addTransformation(bell, [ropeHeight, bellHeight, ropeInBellH](TransformableObject* o, float val) -> void {
            Transform x = *(o->getWorldTransfrom());
            o->transform(Transform::getInverse(x));
            o->translate(0.0f, 0.0f, -bellHeight / 2 + ropeInBellH);
            o->rotateZ(val);
            o->translate(0.0f, 0.0f, bellHeight / 2 - ropeInBellH);
            o->transform(x);
        });
        randomRotations.push_back(rr);
       
        return nodeBell;
    };

    auto addToBell = [&](Skeleton *p, D3DVECTOR transl) {
        Cylinder *rope = new Cylinder(m_d3ddev, ropeRad, ropeHeight, ropeColor);
        rope->rotateX(-D3DX_PI / 2);
        rope->translate(transl.x, transl.y, transl.z);    
        rope->rotateX(D3DX_PI / 2);
        Skeleton *nodeRope = new Skeleton(rope);
        p->addChild(nodeRope);
        Cylinder *bell = new Cylinder(m_d3ddev, bellRad, bellHeight, getRandomColor());
        bell->translate(0.0f, 0.0f, -(ropeHeight2 / 2.0f + bellHeight / 2.0f) + ropeInBellH);

        Skeleton * nodeBell = new Skeleton(bell);
        nodeRope->addChild(nodeBell);
        
        
        RandomRotation *rr = new RandomRotation(-0.06f, 0.06f, 0.002f, 0.003f);
        rr->addTransformation(rope, [ropeHeight, bellRad](TransformableObject* o, float val) -> void {
            Transform x = *(o->getWorldTransfrom());
            o->transform(Transform::getInverse(x));
            o->translate(0.0f, 0.0f, -ropeHeight / 2 + bellRad);
            o->rotateX(val);
            o->translate(0.0f, 0.0f, ropeHeight / 2 - bellRad);
            o->transform(x);
        });
        randomRotations.push_back(rr);
        

        rr = new RandomRotation(-0.03f, 0.03f, 0.002f, 0.003f);
        rr->addTransformation(bell, [bellHeight, ropeInBellH](TransformableObject* o, float val) -> void {
            Transform x = *(o->getWorldTransfrom());
            o->transform(Transform::getInverse(x));
            o->translate(0.0f, 0.0f, -bellHeight / 2.0f + ropeInBellH);
            o->rotateZ(val);
            o->translate(0.0f, 0.0f, bellHeight / 2.0f - ropeInBellH);
            o->transform(x);
        });

        
        randomRotations.push_back(rr);

        
        
        

        return nodeBell;
    };


    Skeleton* bell = addToCross(nodeCrossA, { crossHeight / 2 - bellRad, ropeHeight / 2, 0.0f });
    addToBell(bell, { 0.0f, -(ropeHeight / 2 + bellHeight / 2) + ropeInBellH, 0.0f });

    bell = addToCross(nodeCrossA, { -(crossHeight / 2 - bellRad), ropeHeight / 2, 0.0f });
    addToBell(bell, { 0.0f, -(ropeHeight / 2 + bellHeight / 2) + ropeInBellH, 0.0f });

    addToCross(nodeCrossB, { (crossHeight / 2 - bellRad), ropeHeight / 2, 0.0f });
    addToCross(nodeCrossB, { -(crossHeight / 2 - bellRad), ropeHeight / 2, 0.0f });

    
    RandomRotation *rr = new RandomRotation(-D3DX_PI / 4, D3DX_PI / 4, 0.005f, 0.006f);
    rr->addTransformation(rootRope, [](TransformableObject* o, float val) -> void {
        o->rotateY(val);
    });
    randomRotations.push_back(rr);

    rr = new RandomRotation(-0.1f, 0.1f, 0.001f, 0.002f);
    rr->addTransformation(rootRope, [](TransformableObject* o, float val) -> void {
        o->rotateX(val);
    });
    randomRotations.push_back(rr);
    
    

    

    //


    objects.push_back(mSkeleton);
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
    //delete circleIterator;

    for (RandomRotation *rr : randomRotations) {
        delete rr;
    }
    
    delete wasdCamera;
    delete camera;
    mSkeleton->clean();
    for (RenderableObject *o : objects)
        delete o;

    for (unsigned int i = 0; i < lights.size(); ++i) {
        delete lights[i];
    }
}