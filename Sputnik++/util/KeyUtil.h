#pragma once

#ifdef _WIN32
    #include <windows.h>
    #include <WinUser.h>
    #define KEY_SPACE    VK_SPACE
    #define KEY_LSHIFT   VK_LSHIFT
    #define KEY_RSHIFT   VK_RSHIFT
    #define KEY_LCTRL    VK_LCONTROL
    #define KEY_RCTRL    VK_RCONTROL
    #define KEY_LALT     VK_LMENU
    #define KEY_RALT     VK_RMENU
    #define KEY_TAB      VK_TAB
    #define KEY_ENTER    VK_RETURN
    #define KEY_BACK     VK_BACK
    #define KEY_DELETE   VK_DELETE
    #define KEY_UP       VK_UP
    #define KEY_DOWN     VK_DOWN
    #define KEY_LEFT     VK_LEFT
    #define KEY_RIGHT    VK_RIGHT
    #define KEY_PGUP     VK_PRIOR
    #define KEY_PGDN     VK_NEXT
    #define KEY_F1       VK_F1
    #define KEY_F2       VK_F2
    #define KEY_F3       VK_F3
    #define KEY_F4       VK_F4
    #define KEY_F5       VK_F5
    #define KEY_F6       VK_F6
    #define KEY_F7       VK_F7
    #define KEY_F8       VK_F8
    #define KEY_F9       VK_F9
    #define KEY_F10      VK_F10
    #define KEY_F11      VK_F11
    #define KEY_F12      VK_F12
#else
    #include <X11/keysym.h>
    #define KEY_SPACE    XK_space
    #define KEY_LSHIFT   XK_Shift_L
    #define KEY_RSHIFT   XK_Shift_R
    #define KEY_LCTRL    XK_Control_L
    #define KEY_RCTRL    XK_Control_R
    #define KEY_LALT     XK_Alt_L
    #define KEY_RALT     XK_Alt_R
    #define KEY_TAB      XK_Tab
    #define KEY_ENTER    XK_Return
    #define KEY_BACK     XK_BackSpace
    #define KEY_DELETE   XK_Delete
    #define KEY_UP       XK_Up
    #define KEY_DOWN     XK_Down
    #define KEY_LEFT     XK_Left
    #define KEY_RIGHT    XK_Right
    #define KEY_PGUP     XK_Page_Up
    #define KEY_PGDN     XK_Page_Down
    #define KEY_F1       XK_F1
    #define KEY_F2       XK_F2
    #define KEY_F3       XK_F3
    #define KEY_F4       XK_F4
    #define KEY_F5       XK_F5
    #define KEY_F6       XK_F6
    #define KEY_F7       XK_F7
    #define KEY_F8       XK_F8
    #define KEY_F9       XK_F9
    #define KEY_F10      XK_F10
    #define KEY_F11      XK_F11
    #define KEY_F12      XK_F12
#endif

#include <string>
#include <cctype>


class KeyUtil {
public:
    static std::string getKeyName(int key) {
        if (key <= 0) return "ninguna";

#ifdef _WIN32
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
            case VK_SPACE:    return "espacio";
            case VK_LSHIFT:   return "shift izquierdo";
            case VK_RSHIFT:   return "shift derecho";
            case VK_LCONTROL: return "ctrl izquierdo";
            case VK_RCONTROL: return "ctrl derecho";
            case VK_LMENU:    return "alt izquierdo";
            case VK_RMENU:    return "alt gr";
            case VK_TAB:      return "tab";
            case VK_RETURN:   return "enter";
            case VK_BACK:     return "borrar";
            case VK_DELETE:   return "suprimir";
            case VK_UP:       return "arriba";
            case VK_DOWN:     return "abajo";
            case VK_LEFT:     return "izquierda";
            case VK_RIGHT:    return "derecha";
            case VK_PRIOR:    return "pág. arriba";
            case VK_NEXT:     return "pág. abajo";
            default:
                if (key >= VK_F1 && key <= VK_F24)
                    return "F" + std::to_string(key - VK_F1 + 1);
                return "tecla " + std::to_string(key);
        }

#else
        switch (key) {
            case XK_space:      return "espacio";
            case XK_Shift_L:    return "shift izquierdo";
            case XK_Shift_R:    return "shift derecho";
            case XK_Control_L:  return "ctrl izquierdo";
            case XK_Control_R:  return "ctrl derecho";
            case XK_Alt_L:      return "alt izquierdo";
            case XK_Alt_R:      return "alt gr";
            case XK_Tab:        return "tab";
            case XK_Return:     return "enter";
            case XK_BackSpace:  return "borrar";
            case XK_Delete:     return "suprimir";
            case XK_Up:         return "arriba";
            case XK_Down:       return "abajo";
            case XK_Left:       return "izquierda";
            case XK_Right:      return "derecha";
            case XK_Page_Up:    return "pág. arriba";
            case XK_Page_Down:  return "pág. abajo";
            case XK_Escape:     return "escape";
            default: {
                if (key >= XK_F1 && key <= XK_F24)
                    return "F" + std::to_string(key - XK_F1 + 1);

                if (key >= 0x20 && key <= 0x7E) {
                    char c = static_cast<char>(toupper(key));
                    return std::string(1, c);
                }

                return "tecla " + std::to_string(key);
            }
        }
#endif
    }
};