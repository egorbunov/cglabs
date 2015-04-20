#include "SkyBox.h"

SkyBox::SkyBox(LPDIRECT3DDEVICE9 d3dDevice) {
    this->d3dDevice = d3dDevice;

    TOP_MIPMAP_FILENAMES.push_back(L".\\Resources\\sky_box\\top.png");
    BOTTOM_MIPMAP_FILENAMES.push_back(L".\\Resources\\sky_box\\bottom.png");
    LEFT_MIPMAP_FILENAMES.push_back(L".\\Resources\\sky_box\\left.png");
    RIGHT_MIPMAP_FILENAMES.push_back(L".\\Resources\\sky_box\\right.png");
    BACK_MIPMAP_FILENAMES.push_back(L".\\Resources\\sky_box\\back.png");
    FRONT_MIPMAP_FILENAMES.push_back(L".\\Resources\\sky_box\\front.png");


    top = new TexturedSquare(d3dDevice, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.5f, 0.0f }, 1.0f, TOP_MIPMAP_FILENAMES);
    bottom = new TexturedSquare(d3dDevice, { 0.0f, 1.0f, 0.0f }, { 0.0f, -0.5f, 0.0f }, 1.0f, BOTTOM_MIPMAP_FILENAMES);
    back = new TexturedSquare(d3dDevice, { -1.0f, 0.0f, 0.0f }, { -0.5f, 0.0f, 0.0f }, 1.0f, BACK_MIPMAP_FILENAMES);
    front = new TexturedSquare(d3dDevice, { 1.0f, 0.0f, 0.0f }, { 0.5f, 0.0f, 0.0f }, 1.0f, FRONT_MIPMAP_FILENAMES);
    left = new TexturedSquare(d3dDevice, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.5f }, 1.0f, LEFT_MIPMAP_FILENAMES);
    right = new TexturedSquare(d3dDevice, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -0.5f }, 1.0f, RIGHT_MIPMAP_FILENAMES);
}

SkyBox::~SkyBox() {
    delete top;
    delete bottom;
    delete back;
    delete front;
    delete left;
    delete right;
}

void SkyBox::render(const Transform *worldTransform) {
    top->render(worldTransform);
    bottom->render(worldTransform);
    back->render(worldTransform);
    front->render(worldTransform);
    left->render(worldTransform);
    right->render(worldTransform);
}
