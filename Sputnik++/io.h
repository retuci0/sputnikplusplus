#pragma once

#ifdef _WIN32
    #include <windows.h>
#endif

#include <cstdio>
#include <iostream>

#ifndef _WIN32
    #define FOREGROUND_RED 4
    #define FOREGROUND_GREEN 2
    #define FOREGROUND_INTENSITY 8
#endif


static inline void allocateConsole() {
#ifdef _WIN32
    if (!AllocConsole()) return;

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);

    SetConsoleTitleA("sputnik++");
#else
    // en linux la consola suele estar ya disponible.
#endif
}

static inline void setConsoleColor(int color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE)
        SetConsoleTextAttribute(hConsole, color);
#else
    // códigos ANSI para colores
    int ansiCode = 0;
    if (color == 7) ansiCode = 0; // default
    else if (color == (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)) ansiCode = 33;  // amarillo
    else if (color == (FOREGROUND_RED | FOREGROUND_INTENSITY)) ansiCode = 91;                     // rojo
    printf("\033[%dm", ansiCode);
#endif
}


static inline void info(const char* text) {
    // cerr para que se vea incluso si stdout se redirige a un archivo
    setConsoleColor(7);
    std::cerr << text << std::endl;
}

static inline void warn(const char* text) {
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("%s\n", text);
    setConsoleColor(7);
}

static inline void error(const char* text) {
    setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("%s\n", text);
    setConsoleColor(7);
}