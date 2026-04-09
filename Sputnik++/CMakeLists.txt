file(GLOB SOURCES
    "*.cpp"
    "module/*.cpp"
    "sdk/*.cpp"
)

add_library(SputnikPlusPlus SHARED ${SOURCES})

find_package(Java REQUIRED)
find_package(JNI REQUIRED)

target_include_directories(SputnikPlusPlus PRIVATE
    .
    module
    module/modules
    module/setting
    module/setting/settings
    sdk
    util
    ${JNI_INCLUDE_DIRS}
)

if(WIN32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(SputnikPlusPlus Threads::Threads)
endif()