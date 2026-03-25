#pragma once

#include <windows.h>
#include <cstdio>


void allocateConsole() {
    if (!AllocConsole()) return;

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);

    SetConsoleTitleA("sputnik++");
}

void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE)
        SetConsoleTextAttribute(hConsole, color);
}


void info(const char* text) {
    setConsoleColor(7);
    printf("%s\n", text);
}

void warn(const char* text) {
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("%s\n", text);
    setConsoleColor(7);
}

void error(const char* text) {
    setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("%s\n", text);
    setConsoleColor(7);
}