cmake_minimum_required(VERSION 3.7)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#this needs to be set before the project is built and files are linked etc.

project(3D_SDL)

add_executable(3D_SDL src/cube.c src/3DRlinalg.c src/gfx.c)

#these commands allow the lsp to use autofill for external frameworks
find_package(GSL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

target_link_libraries(3D_SDL PRIVATE GSL::gsl SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image)
