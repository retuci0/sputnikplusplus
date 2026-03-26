#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

const wchar_t* JAVAW_PROCESS = L"javaw.exe";

void getCurrentDir(wchar_t* buffer, size_t size) {
    GetModuleFileNameW(NULL, buffer, (DWORD)size);
    wchar_t* lastSlash = wcsrchr(buffer, L'\\');
    if (lastSlash) *(lastSlash + 1) = L'\0';
}

DWORD getPidByName(const wchar_t* name) {
    DWORD pid = 0;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe = { sizeof(pe) };
        if (Process32FirstW(snap, &pe)) {
            do {
                if (_wcsicmp(pe.szExeFile, name) == 0) {
                    pid = pe.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snap, &pe));
        }
        CloseHandle(snap);
    }
    return pid;
}

int main() {
    wchar_t dllPath[MAX_PATH];
    getCurrentDir(dllPath, MAX_PATH);
    wcscat_s(dllPath, L"Sputnik++.dll");

    std::cout << "esperando a Minecraft" << std::endl;
    DWORD pid = 0;
    while (!pid) {
        pid = getPidByName(JAVAW_PROCESS);
        for (int i = 0; i < 3; i++) {
            std::cout << ".";
            Sleep(500);
        }
        std::cout << "\b\b\b   \b\b\b" << std::flush;
        Sleep(500);
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) {
        std::cerr << "no se pudo abrir el proceso de Minecraft: " << GetLastError() << std::endl;
        std::cout << "presiona enter para salir...";
        std::cin.ignore();
        return -1;
    }

    // alocar memoria virtual
    size_t dllBytes = (wcslen(dllPath) + 1) * sizeof(wchar_t);
    void* remoteMem = VirtualAllocEx(hProc, NULL, dllBytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMem) {
        std::cerr << "VirtualAllocEx ha fallado: " << GetLastError() << std::endl;
        std::cout << "presiona enter para salir...";
        CloseHandle(hProc);
        return -1;
    }

    if (!WriteProcessMemory(hProc, remoteMem, dllPath, dllBytes, NULL)) {
        std::cerr << "WriteProcessMemory ha fallado: " << GetLastError() << std::endl;
        std::cout << "presiona enter para salir...";
        VirtualFreeEx(hProc, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return -1;
    }

    // crear hilo remoto para cargar el DLL
    HANDLE hThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, remoteMem, 0, NULL);
    if (!hThread) {
        std::cerr << "CreateRemoteThread ha fallado: " << GetLastError() << std::endl;
        std::cout << "presiona enter para salir...";
        VirtualFreeEx(hProc, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return -1;
    }

    WaitForSingleObject(hThread, 5000);

    // cleanup
    CloseHandle(hThread);
    VirtualFreeEx(hProc, remoteMem, 0, MEM_RELEASE);
    CloseHandle(hProc);

    std::cout << "inyectado correctamente!" << std::endl;
    std::cout << "presiona enter para salir...";
    std::cin.ignore();
    return 0;
}