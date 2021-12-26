cmake_minimum_required(VERSION 3.10)
project(raytracer)
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "./src/*.cpp")
add_executable(raytracer ${SOURCES})
set_property(TARGET raytracer PROPERTY CXX_STANDARD 20)

#Optional:
include_directories(include)
#target_link_libraries(lang LLVM)

