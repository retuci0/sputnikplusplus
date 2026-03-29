#include "java.h"
#include "io.h"

#include "module/ModuleManager.h"

#include <jni.h>
#include <thread>
#include <chrono>


static void tick() {
    // gestión de teclas
    for (int key = 1; key < 256; ++key) {
        if (GetAsyncKeyState(key) & 1)
            ModuleManager::getInstance()->onKey(key, 1);
    }

    ModuleManager::getInstance()->tick();

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

static void run() {
    allocateConsole();

    Java::jvm = getJVM();
    if (!Java::jvm) {
        error("JVM no encontrado");
        return;
    }

    ModuleManager::getInstance()->addModules();

    while (true) {
        tick();
    }
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(nullptr, 0,
            [](LPVOID) -> DWORD { run(); return 0; },
            nullptr, 0, nullptr
        );
        if (hThread) CloseHandle(hThread);
    } else if (reason == DLL_PROCESS_DETACH) {
        FreeConsole();
    }
    return TRUE;
}