#ifdef _WIN32
    #include "windows.h"
#else
    #include "linux.h"
#endif

int main() {
    return run_injector();
}