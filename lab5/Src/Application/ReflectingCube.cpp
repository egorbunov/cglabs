#include "ReflectingCube.h"

static std::vector<LPCWSTR> getMipMapFilenames(int i) {
    LPWSTR str = new WCHAR[100];
    swprintf(str, 100, L".\\Resources\\f%d.png", i);
    
    std::vector<LPCWSTR> res;
    res.push_back(str);

    return res;
}
const std::vector<LPCWSTR> ReflectingCube::FACE_1_TEXS = getMipMapFilenames(1);
const std::vector<LPCWSTR> ReflectingCube::FACE_2_TEXS = getMipMapFilenames(2);
const std::vector<LPCWSTR> ReflectingCube::FACE_3_TEXS = getMipMapFilenames(3);
const std::vector<LPCWSTR> ReflectingCube::FACE_4_TEXS = getMipMapFilenames(4);
const std::vector<LPCWSTR> ReflectingCube::FACE_5_TEXS = getMipMapFilenames(5);
const std::vector<LPCWSTR> ReflectingCube::FACE_6_TEXS = getMipMapFilenames(6);



ReflectingCube::ReflectingCube(LPDIRECT3DDEVICE9 d3dDevice) {
    device = d3dDevice;


    faces.push_back(new TexturedSquare(device, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.5f, 0.0f }, 1.0f, FACE_1_TEXS, 128, false));
    faces.push_back(new TexturedSquare(device, { 0.0f, -1.0f, 0.0f }, { 0.0f, -0.5f, 0.0f }, 1.0f, FACE_2_TEXS, 128, false));
    faces.push_back(new TexturedSquare(device, { 1.0f, 0.0f, 0.0f }, { -0.5f, 0.0f, 0.0f }, 1.0f, FACE_3_TEXS, 128, false));
    faces.push_back(new TexturedSquare(device, { 1.0f, 0.0f, 0.0f }, { 0.5f, 0.0f, 0.0f }, 1.0f, FACE_4_TEXS, 128, false));
    faces.push_back(new TexturedSquare(device, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.5f }, 1.0f, FACE_5_TEXS, 128, false));
    faces.push_back(new TexturedSquare(device, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -0.5f }, 1.0f, FACE_6_TEXS, 128, false));

    LPD3DXBUFFER pErrors = NULL;
    LPD3DXBUFFER pShaderBuff = NULL;
    LPD3DXCONSTANTTABLE pConstTableVS = NULL;
    LPD3DXCONSTANTTABLE pConstTablePS = NULL;

    D3DVERTEXELEMENT9 g_aVertDecl[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };

    device->CreateVertexDeclaration(g_aVertDecl, &m_vertexDeclaration);

    
    ID3DXBuffer *errors;
    HRESULT hr = D3DXCreateEffectFromFile(d3dDevice, L".\\shader\\effect.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL, &effect, &errors);
    
    if (hr != D3D_OK || errors)
    {
        char *str = (char *)errors->GetBufferPointer();

        MessageBox(NULL, L"Cannot load effect", L"ERROR", MB_ICONERROR | MB_OK);
        PostQuitMessage(EXIT_FAILURE);
        errors->Release();
    }


    hr = device->CreateCubeTexture(256,
                                   1,
                                   D3DUSAGE_RENDERTARGET,
                                   D3DFMT_A16B16G16R16F,
                                   D3DPOOL_DEFAULT,
                                   &cubeEnvTexture,
                                   NULL);

    fresnelPow = 2.0f;
                                

}

ReflectingCube::~ReflectingCube() {
    for (TexturedSquare* f : faces) {
        delete f;
    }
    effect->Release();
    cubeEnvTexture->Release();
    m_vertexDeclaration->Release();

    
    for (LPCWSTR s : FACE_1_TEXS)
        delete s;
    for (LPCWSTR s : FACE_2_TEXS)
        delete s;
    for (LPCWSTR s : FACE_3_TEXS)
        delete s;
    for (LPCWSTR s : FACE_4_TEXS)
        delete s;
    for (LPCWSTR s : FACE_5_TEXS)
        delete s;
    for (LPCWSTR s : FACE_6_TEXS)
        delete s;
}

void ReflectingCube::renderObjectsToCubeMap(std::vector<RenderableObject*> objects) {
    HRESULT hr;

    device->EndScene();

    // Save transformation matrices of the device
    D3DXMATRIX matProjSave, matViewSave;
    device->GetTransform(D3DTS_VIEW, &matViewSave);
    device->GetTransform(D3DTS_PROJECTION, &matProjSave);

    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 11000.0f);
    device->SetTransform(D3DTS_PROJECTION, &matProj);

    D3DXMATRIX mViewDir = matViewSave;
    mViewDir._41 = mViewDir._42 = mViewDir._43 = 0.0f;

    LPDIRECT3DSURFACE9 pRTOld = NULL;
    hr = device->GetRenderTarget(0, &pRTOld);
    if (FAILED(hr)) {MessageBox(NULL, L"Error: get render target", L"Error", MB_OK);}

    LPDIRECT3DSURFACE9 pDSOld = NULL;
    if (SUCCEEDED(device->GetDepthStencilSurface(&pDSOld))) {
        // ok
    }

    for (int nFace = 0; nFace < 6; ++nFace)
    {
        LPDIRECT3DSURFACE9 pSurf;

        hr = cubeEnvTexture->GetCubeMapSurface((D3DCUBEMAP_FACES)nFace, 0, &pSurf);
        hr = device->SetRenderTarget(0, pSurf);
        if (FAILED(hr)) { MessageBox(NULL, L"Error: render targer or cube map...", L"Error", MB_OK); }

        D3DXMATRIXA16 mView = DXUTGetCubeMapViewMatrix(nFace);
        D3DXMatrixMultiply(&mView, &mViewDir, &mView);
        device->SetTransform(D3DTS_VIEW, &mView);

        hr = device->Clear(0L, NULL, D3DCLEAR_ZBUFFER,
                      0x000000ff, 1.0f, 0L);

        if (SUCCEEDED(device->BeginScene()))
        {
            for (RenderableObject *ro : objects) {
                ro->render(ro->getWorldTransfrom());
            }

            device->EndScene();
        }

        SAFE_RELEASE(pSurf);

    }

    hr = device->Clear(0L, NULL, D3DCLEAR_ZBUFFER,
                       0x000000ff, 1.0f, 0L);

    // Restore depth-stencil buffer and render target
    if (pDSOld)
    {
        device->SetDepthStencilSurface(pDSOld);
        SAFE_RELEASE(pDSOld);
    }
    device->SetRenderTarget(0, pRTOld);
    SAFE_RELEASE(pRTOld);

    // Restore the original transformation matrices
    device->SetTransform(D3DTS_VIEW, &matViewSave);
    device->SetTransform(D3DTS_PROJECTION, &matProjSave);

    device->BeginScene();
}

void ReflectingCube::render(const Transform *worldTransform) {
    HRESULT hr;

    device->SetVertexDeclaration(m_vertexDeclaration);
    D3DXMATRIX viewMat, projMat;
    device->GetTransform(D3DTS_VIEW, &viewMat);
    device->GetTransform(D3DTS_PROJECTION, &projMat);

    D3DXMatrixMultiply(&viewMat, worldTransform->getTransformMatrix(), &viewMat);

    hr = effect->SetMatrix("g_worldViewMat", &viewMat);
    hr = effect->SetMatrix("g_projMat", &projMat);
    hr = effect->SetTexture("g_txCubeMap", cubeEnvTexture);
    hr = effect->SetFloat("g_fresPow", fresnelPow);
    hr = effect->SetTechnique("mirror_technique");


    std::string s = "faceTex";
    
    Transform t;
    for (TexturedSquare* f : faces) {
        size_t num_passes = 0;

        hr = effect->SetTexture(s.c_str(), f->getTexture());

        hr = effect->Begin(&num_passes, 0);

        for (size_t i = 0; i < num_passes; ++i)
        {
            hr = effect->BeginPass(i);
            f->render(&t);
            hr = effect->EndPass();
        }
        hr = effect->End();

    }

    
}

void ReflectingCube::incFresnelPow() {
    if (fresnelPow + 0.25f <= 10.0f)
        fresnelPow += 0.25f;
}
void ReflectingCube::decFresnelPow() {
    if (fresnelPow - 0.25f >= 0.0f)
        fresnelPow -= 0.25f;
}

