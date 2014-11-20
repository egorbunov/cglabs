#ifndef COMMON_UTILS_H_INCLUDED__
#define COMMON_UTILS_H_INCLUDED__

#include <windows.h>
#include <string>
#include <d3dx9.h>

struct CUSTOMVERTEX { FLOAT X, Y, Z; float nx, ny, nz; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL)

inline std::wstring convertToWstring(std::string str) {
    return std::wstring(str.begin(), str.end());
}

inline D3DXCOLOR hexToColor(DWORD color) {
    return (D3DXCOLOR(
        ((color >> 24) & ~((-1) << 8)) / 255.0f,
        ((color >> 16) & ~((-1) << 8)) / 255.0f,
        ((color >> 8) & ~((-1) << 8)) / 255.0f,
        ((color) & ~((-1) << 8)) / 255.0f));
}

#endif