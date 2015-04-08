#ifndef COMMON_UTILS_H_INCLUDED__
#define COMMON_UTILS_H_INCLUDED__

#include <windows.h>
#include <string>
#include <d3dx9.h>

#include <random>
#include <vector>

inline D3DXCOLOR hexToColor(DWORD color);

namespace {
    std::vector<D3DXCOLOR> gColors = {
        hexToColor(0x79ef4b), hexToColor(0xa900c1), hexToColor(0x7ae08d), hexToColor(0x79ef4b), hexToColor(0xd7e480), hexToColor(0xaf696c), hexToColor(0x074571), hexToColor(0x1050d6),
        hexToColor(0xf3571f), hexToColor(0xffa133), hexToColor(0x783c4d), hexToColor(0x422760), hexToColor(0xf3475c), hexToColor(0x79ef4b), hexToColor(0x8b7772), hexToColor(0x8cee1d),
        hexToColor(0x478713), hexToColor(0xb86ce2), hexToColor(0x79ef4b), hexToColor(0x18048e), hexToColor(0xa04272), hexToColor(0xbce430), hexToColor(0x74dbb7), hexToColor(0xe05aee),
        hexToColor(0xf9d017), hexToColor(0x142543), hexToColor(0x20481e), hexToColor(0xf13bfc), hexToColor(0x0c2204), hexToColor(0x5bb610), hexToColor(0x79ef4b), hexToColor(0xf95743)
    };
}

inline D3DXCOLOR getRandomColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, gColors.size() - 1);
    int index = dis(gen);
    return gColors.at(index);
}

struct CUSTOMVERTEX { FLOAT X, Y, Z; float nx, ny, nz; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL)

inline std::wstring convertToWstring(std::string str) {
    return std::wstring(str.begin(), str.end());
}

inline D3DXCOLOR hexToColor(DWORD color) {
    return (D3DXCOLOR(
        ((color >> 16) & ~((-1) << 8)) / 255.0f,
        ((color >> 8) & ~((-1) << 8)) / 255.0f,
        ((color) & ~((-1) << 8)) / 255.0f,
        1.0f));
}



#endif