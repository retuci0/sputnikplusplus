#include "java.h"
#include "io.h"

#include "module/ModuleManager.h"

#include <jni.h>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
#else
    #include "util/LinuxKeys.h"
#endif


static void tick() {
#ifdef _WIN32
    for (int key = 1; key < 256; ++key) {
        if (GetAsyncKeyState(key) & 1)
            ModuleManager::getInstance()->onKey(key, 1);
    }
#else
    LinuxKeys::pollKeys([](int keysym) {
        ModuleManager::getInstance()->onKey(keysym, 1);
    });
#endif

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

    info("Sputnik++ init'd");
    ModuleManager::getInstance()->addModules();

    while (true) {
        tick();
    }
}

#ifdef _WIN32
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
#else
__attribute__((constructor)) void init() {
    std::thread([](){ run(); }).detach();
}

__attribute__((destructor)) void fini() {
    // cleanup??
}
#endif