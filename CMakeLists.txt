cmake_minimum_required(VERSION 3.10.2)

project("en2" LANGUAGES C CXX)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Please build out of tree")
endif()

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

set (EN2_SOURCES
    src/misc.h
    src/fluid.cpp
    src/fluid.h
)
    
set (EN2_MECHANICS_SOURCES
    src/mechanics/mechanic.h
    src/mechanics/mechanics.h
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

add_executable(en2 src/main.cpp ${EN2_SOURCES} ${EN2_MECHANICS_SOURCES})
target_compile_features(en2 PRIVATE cxx_std_17)
target_include_directories(en2 PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(en2 ${SDL2_LIBRARIES})

add_executable(test_en2 test/test.cpp ${EN2_SOURCES} ${EN2_MECHANICS_SOURCES})
target_compile_features(test_en2 PRIVATE cxx_std_17)
target_include_directories(test_en2 PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(test_en2 PRIVATE include src)
