#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <cstdlib>
#include <sys/stat.h>
#include <thread>
#include <chrono>

const std::string TARGET_PROCESS = "java";
const std::string SO_NAME = "libSputnikPlusPlus.so";


// obtener la ruta actual
inline std::string getCurrentDir() {
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return std::string(buffer);
    }
    return "";
}

// obtener el PID a partir del nombre de un proceso
inline pid_t getPidByName(const std::string& name) {
    DIR* dir = opendir("/proc");
    if (!dir) return 0;

    dirent* entry;
    pid_t result = 0;

    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type != DT_DIR) continue;
        pid_t pid = atoi(entry->d_name);
        if (pid == 0) continue;

        std::string cmdline_path = "/proc/" + std::string(entry->d_name) + "/comm";
        std::ifstream comm_file(cmdline_path);
        std::string comm;
        if (comm_file >> comm) {
            if (comm == name) {
                result = pid;
                break;
            }
        }
    }
    closedir(dir);
    return result;
}

// inyectar el SO usando gdb
inline bool inject(pid_t pid, const std::string& so_path) {
    std::string command = "gdb -n -q -batch -p " + std::to_string(pid) +
                          " -ex 'call (void*)dlopen(\"" + so_path + "\", 2)' "
                          " -ex 'detach' -ex 'quit' 2>&1";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return false;

    char buffer[128];
    std::string output;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    int ret = pclose(pipe);

    if (ret == 0 && output.find("$1 =") != std::string::npos) {
        return true;
    }
    std::cerr << "inyección gdb fallida:\n" << output << std::endl;
    return false;
}

inline int run_injector() {
    std::string so_path = getCurrentDir() + "/" + SO_NAME;

    struct stat buffer;
    if (stat(so_path.c_str(), &buffer) != 0) {
        std::cerr << "error: " << SO_NAME << " no se encuentra en el directorio actual." << std::endl;
        std::cout << "presiona enter para salir...";
        std::cin.ignore();
        return -1;
    }

    std::cout << "esperando a Minecraft ";
    pid_t pid = 0;
    while (pid == 0) {
        pid = getPidByName(TARGET_PROCESS);
        if (pid == 0) {
            for (int i = 0; i < 3; ++i) {
                std::cout << "." << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            std::cout << "\b\b\b   \b\b\b" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    std::cout << TARGET_PROCESS << " fue encontrado con el PID " << pid << std::endl;
    std::cout << "inyectando " << SO_NAME << " ..." << std::endl;

    if (inject(pid, so_path)) {
        std::cout << "inyectado correctamente!" << std::endl;
    } else {
        std::cerr << "inyección fallida." << std::endl;
    }

    std::cout << "presiona enter para salir...";
    std::cin.ignore();
    return 0;
}
