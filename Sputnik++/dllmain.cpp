#include "java.h"
#include "io.h"

#include "module/ModuleManager.h"

#include <jni.h>
#include <thread>
#include <chrono>


void tick() {
    Java::env = getJNIEnv();
    if (!Java::env) {
        error("jni env no encontrado");
        return;
    }

    for (int key = 1; key < 256; ++key) {
        if (GetAsyncKeyState(key) & 1) {
            ModuleManager::getInstance()->onKey(key, 1);
        }
    }

    ModuleManager::getInstance()->tick();

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void run() {
    allocateConsole();

    Java::jvm = getJVM();
    if (!Java::jvm) {
        error("JVM no encontrado");
        return;
    }

    Java::env = getJNIEnv();
    if (!Java::env) {
        error("entorno JNI no encontrado");
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
            [](LPVOID) -> DWORD {
                run(); return 0;
            },
            nullptr, 0, nullptr
        );
        if (hThread) CloseHandle(hThread);
    } else if (reason == DLL_PROCESS_DETACH) {
        FreeConsole();
    }

    return TRUE;
}