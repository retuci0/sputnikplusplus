#pragma once

#ifndef _WIN32
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <functional>
#include <cstring>

class LinuxKeys {
public:
    // llama a callback(keysym) por cada tecla recién pulsada desde la última llamada
    static void pollKeys(std::function<void(int)> callback) {
        Display* dpy = getDisplay();
        if (!dpy) return;

        char keys[32] = {};
        XQueryKeymap(dpy, keys);

        for (int kc = 8; kc < 256; ++kc) {
            bool pressed  = (keys[kc / 8] >> (kc % 8)) & 1;
            bool wasPrev  = (prev[kc / 8] >> (kc % 8)) & 1;

            if (pressed && !wasPrev) {
                KeySym ks = XKeycodeToKeysym(dpy, kc, 0);
                if (ks != NoSymbol)
                    callback(static_cast<int>(ks));
            }
        }

        std::memcpy(prev, keys, 32);
    }

private:
    static Display* getDisplay() {
        static Display* dpy = XOpenDisplay(nullptr);
        return dpy;
    }

    static inline char prev[32] = {};
};
#endif