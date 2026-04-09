add_executable(injector main.cpp)

target_include_directories(injector PRIVATE .)

if(WIN32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(injector Threads::Threads)
endif()