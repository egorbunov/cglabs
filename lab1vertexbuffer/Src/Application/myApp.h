#ifndef __MYLAPP_H__632619789336566350
#define __MYLAPP_H__632619789336566350

/**
@file     cglapp.h
@brief    Class cglApp definition
@date     Created on 10/09/2005
@project  D3DBase
@author   Bvs
*/
#define D3D_DEBUG_INFO
// *******************************************************************
// includes

#include <d3d9.h>
#include <d3dx9.h>
#include "Library/cglApp.h"

#include "commonUtils.h"
#include "Thorus.h"
#include "CartesianCoordinateSystem.h"
#include "Camera.h"

// *******************************************************************
// defines & constants

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// *******************************************************************
// classes 

// Application class
class myApp : public cglApp
{
public:
    // Constructor
    myApp(int nW, int nH, void* hInst, int nCmdShow);

    // Destructor
    virtual ~myApp() {}

    // This function performs input processing. Returns true if input is handled
    virtual bool processInput(unsigned int nMsg, int wParam, long lParam);

    virtual void renderInternal();

protected:
    virtual void update();

private:
    Camera camera;

    const D3DXVECTOR3 thorusCenter;
    Thorus thorus;
    CartesianCoordinateSystem thorusCoordSystem;
    CartesianCoordinateSystem globalCoordSystem;


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
