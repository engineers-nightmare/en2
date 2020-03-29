cmake_minimum_required(VERSION 3.10.2)

project("test" LANGUAGES C CXX)

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

set (TEST_SOURCES
    test.cpp
)

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

add_executable(test_en2 ${TEST_SOURCES})
target_compile_features(test_en2 PRIVATE cxx_std_17)
target_link_libraries(test_en2 simulation)
target_include_directories(test_en2 PRIVATE ../include ../simulation)
