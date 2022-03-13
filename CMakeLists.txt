cmake_minimum_required(VERSION 3.22.1)

project(pbdoom)

find_package(Inkview REQUIRED)
include_directories(${INKVIEW_INCLUDE_DIR})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIR_freetype2})

add_executable(pbdoom.app main.cpp)
target_link_libraries(pbdoom.app ${INKVIEW_LIBRARIES})
