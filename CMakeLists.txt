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
    src/main.cpp
)

add_subdirectory(simulation)
add_subdirectory(test)
add_subdirectory(imgui_en2)

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

add_executable(en2 src/main.cpp ${EN2_SOURCES})
target_compile_features(en2 PRIVATE cxx_std_17)
target_include_directories(en2 PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(en2 simulation ${SDL2_LIBRARIES})
target_include_directories(en2 PRIVATE include simulation)
