cmake_minimum_required(VERSION 3.10.2)

project("imgui_en2" LANGUAGES C CXX)

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
    
set (IMGUIRENDER_SOURCES
    main.cpp
    imgui_sdl.h
    imgui_sdl.cpp
    imgui/imconfig.h
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/imgui_impl_sdl.h
    imgui/imgui_impl_sdl.cpp
    imgui/imnodes.h
    imgui/imnodes.cpp
)

add_executable(imgui_en2 ${IMGUIRENDER_SOURCES})
target_compile_features(imgui_en2 PRIVATE cxx_std_17)
target_link_libraries(imgui_en2 simulation ${SDL2_LIBRARIES})
target_include_directories(imgui_en2 PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(imgui_en2 PRIVATE ../include ../simulation)
