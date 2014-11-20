#ifndef __MYLAPP_H__632619789336566350
#define __MYLAPP_H__632619789336566350

/**
@file     cglapp.h
@brief    Class cglApp definition
@date     Created on 10/09/2005
@project  D3DBase
@author   Bvs
*/

// *******************************************************************
// includes

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "Library/cglApp.h"

#include "commonUtils.h"
#include "CartesianCoordinateSystem.h"
#include "Camera.h"
#include "XMeshObject.h"
#include "CircleIterator.h"
#include "WASDCamera.h"
#include "LightSource.h"
#include "DirectedLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Plane.h"



// *******************************************************************
// defines & constants

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

#define CAMERA_SHIFT_STEP 8

// *******************************************************************
// classes 

// Application class
class myApp : public cglApp
{
public:
    // Constructor
    myApp(int nW, int nH, void* hInst, int nCmdShow);

    // Destructor
    virtual ~myApp();

    // This function performs input processing. Returns true if input is handled
    virtual bool processInput(unsigned int nMsg, int wParam, long lParam);

    virtual void renderInternal();

protected:
    virtual void update();

private:
    Camera camera;
    WASDCamera wasdCamera;
    CartesianCoordinateSystem globalCoordSystem;
    XMeshObject car;
    CircleIterator *circleIterator;
    Plane plane;
    bool isWASDCameraActive;

    D3DMATERIAL9 globalMaterial;

    // lights
    std::vector<LightSource*> lights;


    enum
    {
        MAX_KEYS = 128
    };

    int   m_nPrevMouseX;
    int   m_nPrevMouseY;
    bool  m_keysPressed[MAX_KEYS];

    void rotate(float dx, float dy);
    void rotateCamera(float dx, float dy);
    void zoom(float dr);

    void init_graphics();

    LPDIRECT3DDEVICE9 m_d3ddev;    // the pointer to the device class
    LPDIRECT3DVERTEXBUFFER9 m_vbuffer = NULL;
    D3DXMATRIX m_matWorld;
    D3DXMATRIX m_matView;
    D3DXMATRIX m_matProj;
};


// *******************************************************************
// functions 

#endif //__CGLAPP_H__632619789336566350
