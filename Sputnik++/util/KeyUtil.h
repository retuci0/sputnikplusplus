#pragma once

#include <string>
#include <windows.h>
#include <WinUser.h>


class KeyUtil {
public:
    static const std::string getKeyName(int key) {
        if (key <= 0) return "ninguna";

        UINT scanCode = MapVirtualKeyA(key, MAPVK_VK_TO_VSC);
        DWORD flags = 0;

        if (key >= VK_PRIOR && key <= VK_DOWN) flags |= 0x100;

        char buffer[256];
        int ret = GetKeyNameTextA((scanCode << 16) | flags, buffer, sizeof(buffer));
        if (ret > 0) {
            for (char* p = buffer; *p; ++p) *p = toupper(*p);
            return std::string(buffer);
        }

        switch (key) {
            case VK_SPACE:      return "espacio";
            case VK_LSHIFT:     return "shift izquierdo";
            case VK_RSHIFT:     return "shift derecho";
            case VK_LCONTROL:   return "ctrl izquierdo";
            case VK_RCONTROL:   return "ctrl derecho";
            case VK_LMENU:      return "alt izquierdo";
            case VK_RMENU:      return "alt gr";
            case VK_TAB:        return "tab";
            case VK_RETURN:     return "enter";
            case VK_BACK:       return "borrar";
            case VK_DELETE:     return "suprimir";
            case VK_UP:         return "arriba";
            case VK_DOWN:       return "abajo";
            case VK_LEFT:       return "izquierda";
            case VK_RIGHT:      return "derecha";
            case VK_PRIOR:      return "pág. arriba";
            case VK_NEXT:       return "pág. abajo";

            default: {
                if (key >= VK_F1 && key <= VK_F24) {
                    int fnum = key - VK_F1 + 1;
                    return "F" + std::to_string(fnum);
                }

                return "tecla " + std::to_string(key);
            }
        }
    }
};