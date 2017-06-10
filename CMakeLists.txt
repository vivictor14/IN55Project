cmake_minimum_required(VERSION 3.7)
project(IN55Project)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH C:/Qt/5.6/mingw49_32)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(APP_MODELS src/models)
set(APP_WIDGETS src/widgets)
set(APP_UTIL src/util)
set(APP_CONTROLS src/controls)

include_directories( ${APP_MODELS} )
include_directories( ${APP_WIDGETS} )
include_directories( ${APP_UTIL} )
include_directories( ${APP_CONTROLS} )

find_package( Qt5Widgets REQUIRED )
find_package( Qt5OpenGL )
find_package( OpenGL )



set(SOURCE_FILES src/main.cpp
        ${APP_MODELS}/stb_image.h
        ${APP_MODELS}/gem.h
        ${APP_MODELS}/gem.cpp
        ${APP_MODELS}/skyBox.h
        ${APP_MODELS}/skyBox.cpp
        ${APP_WIDGETS}/displayWidget.h
        ${APP_WIDGETS}/displayWidget.cpp
        ${APP_WIDGETS}/menuWidget.h
        ${APP_WIDGETS}/menuWidget.cpp
        ${APP_UTIL}/vertex.h
        ${APP_UTIL}/transform3D.h
        ${APP_UTIL}/transform3D.cpp
        ${APP_CONTROLS}/camera3D.h
        ${APP_CONTROLS}/camera3D.cpp
        ${APP_CONTROLS}/input.h
        ${APP_CONTROLS}/input.cpp src/models/stb_image.h src/models/skyBox.cpp src/models/skyBox.h)

add_executable(IN55Project ${SOURCE_FILES})

target_link_libraries(IN55Project Qt5::Widgets Qt5::OpenGL)