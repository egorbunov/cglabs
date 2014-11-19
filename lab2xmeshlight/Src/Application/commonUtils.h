#ifndef COMMON_UTILS_H_INCLUDED__
#define COMMON_UTILS_H_INCLUDED__

#include <windows.h>
#include <string>

struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

inline std::wstring convertToWstring(std::string str) {
    return std::wstring(str.begin(), str.end());
}

#endif