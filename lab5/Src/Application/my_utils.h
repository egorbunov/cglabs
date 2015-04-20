#ifndef MY_UTILS_H_INCLUDED__
#define MY_UTILS_H_INCLUDED__

#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>

#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)

D3DXMATRIX WINAPI DXUTGetCubeMapViewMatrix(DWORD dwFace);

std::string GetFileAsString(const std::string& strFileName);

#endif