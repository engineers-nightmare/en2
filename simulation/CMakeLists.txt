cmake_minimum_required(VERSION 3.10.2)

project("simulation" LANGUAGES C CXX)

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
    
set (SIMULATION_SOURCES
    mechanics/mechanic.h
    mechanics/mechanics.h
    mechanics/tank.h
    mechanics/tank.cpp
    mechanics/pump.h
    mechanics/pump.cpp
    mechanics/combinator.h
    mechanics/combinator.cpp
    mechanics/torch.h
    mechanics/torch.cpp
    fluid.h
    fluid.cpp   
)

add_library(simulation ${SIMULATION_SOURCES})
target_compile_features(simulation PRIVATE cxx_std_17)
target_include_directories(simulation PRIVATE ../include)
