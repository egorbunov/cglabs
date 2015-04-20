#ifndef SKY_BOX_H_INCLUDED__
#define SKY_BOX_H_INCLUDED__

#include <vector>
#include <math.h>
#include <stdlib.h>
#include <windows.h>

#include "texture_objects.h"

class SkyBox : public RenderableObject {
private:
    PDIRECT3DDEVICE9 d3dDevice;

    std::vector<LPCWSTR> TOP_MIPMAP_FILENAMES;
    std::vector<LPCWSTR> BOTTOM_MIPMAP_FILENAMES;
    std::vector<LPCWSTR> FRONT_MIPMAP_FILENAMES;
    std::vector<LPCWSTR> BACK_MIPMAP_FILENAMES;
    std::vector<LPCWSTR> LEFT_MIPMAP_FILENAMES;
    std::vector<LPCWSTR> RIGHT_MIPMAP_FILENAMES;

    TexturedSquare *top;
    TexturedSquare *bottom;
    TexturedSquare *front;
    TexturedSquare *back;
    TexturedSquare *left;
    TexturedSquare *right;

public:
    SkyBox(LPDIRECT3DDEVICE9 d3dDevice);
    ~SkyBox();

    void render(const Transform *worldTransform);


};

#endif