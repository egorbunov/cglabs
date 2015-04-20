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
#include <vector>
#include <sstream>
#include <iomanip>

#include "Library/cglApp.h"

#include "commonUtils.h"
#include "CartesianCoordinateSystem.h"
#include "Camera.h"
#include "CircleIterator.h"
#include "WASDCamera.h"
#include "LightSource.h"
#include "DirectedLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Plane.h"
#include "Cylinder.h"
#include "SkyBox.h"
#include "XMeshObject.h"
#include "ReflectingCube.h"
#include "my_utils.h"

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
    void doTransformations();

private:
    static const int MIPMAPS[3];
    static const int MIN_MAG[2];
    int mimpmapIndex;
    int minIndex;
    int magIndex;
    float mipMapBias;

    // camera stuff
    bool isWASDCameraActive;
    Camera *camera;
    WASDCamera *wasdCamera;

    // iterator for rotating something...
    CircleIterator *circleIterator;
    D3DMATERIAL9 globalMaterial;

    // lights
    std::vector<LightSource*> lights;

    ReflectingCube *reflectingCube;
    std::vector<RenderableObject*> objects;
    std::vector<RenderableObject*> aroundCubeObjects;


    enum
    {
        MAX_KEYS = 200
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

    void prepareLights();
};


// *******************************************************************
// functions 

#endif //__CGLAPP_H__632619789336566350
