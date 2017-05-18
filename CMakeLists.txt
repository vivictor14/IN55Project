cmake_minimum_required(VERSION 3.7)
project(IN55Project)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH C:/Qt/5.6/mingw49_32)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(APP_MODELS src/models)
set(APP_WINDOWS src/windows)
set(APP_UTIL src/util)

include_directories( ${APP_MODELS} )
include_directories( ${APP_WINDOWS} )
include_directories( ${APP_UTIL} )

find_package( Qt5Widgets REQUIRED )
find_package( Qt5OpenGL )
find_package( OpenGL )



set(SOURCE_FILES src/main.cpp
        ${APP_MODELS}/gem.h
        ${APP_MODELS}/gem.cpp
        ${APP_WINDOWS}/displayWindow.h
        ${APP_WINDOWS}/displayWindow.cpp
        ${APP_UTIL}/vertex.h)

add_executable(IN55Project ${SOURCE_FILES})

target_link_libraries(IN55Project Qt5::Widgets Qt5::OpenGL)