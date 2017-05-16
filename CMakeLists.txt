cmake_minimum_required(VERSION 3.7)
project(IN55Project)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH C:/Qt/5.6/mingw49_32)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package( Qt5Widgets REQUIRED )

set(SOURCE_FILES main.cpp)
add_executable(IN55Project ${SOURCE_FILES})

target_link_libraries(IN55Project Qt5::Widgets)