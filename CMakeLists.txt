cmake_minimum_required(VERSION 3.10.2)

project("en2" LANGUAGES C CXX)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Please build out of tree")
endif()

set (EN2_SOURCES
    src/main.cpp
)
    
set (EN2_MECHANICS_SOURCES
    src/mechanics/mechanic.h
    src/mechanics/tank.h
    src/mechanics/tank.cpp
    src/mechanics/pump.h
    src/mechanics/pump.cpp
    src/mechanics/combinator.h
    src/mechanics/combinator.cpp
    src/mechanics/torch.h
    src/mechanics/torch.cpp
)

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

add_executable(en2 ${EN2_SOURCES} ${EN2_MECHANICS_SOURCES})
target_compile_features(en2 PRIVATE cxx_std_17)
target_include_directories(en2 PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(en2 ${SDL2_LIBRARIES})
