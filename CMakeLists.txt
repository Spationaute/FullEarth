cmake_minimum_required(VERSION 3.10.2)
project(FullEarth)
set(CMAKE_CXX_STANDARD 14)
set(OpenGL_GL_PREFERENCE GLVND)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
 #   message(STATUS "Preparing for Windows Platfom")
    set(CMAKE_FIND_ROOT_PATH "/usr/x86_64-w64-mingw32")
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

    find_package(CURL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(FLTK REQUIRED)
   # find_package(OpenGL REQUIRED)

else()
    find_package(FLTK REQUIRED)
    find_package(CURL REQUIRED)
    find_package(GLEW REQUIRED)
   # find_package(OpenGL REQUIRED)
endif()

set(SOURCES src/main.cpp src/FullEarth.cpp src/FullEarth.h src/Geojson.cpp src/Geojson.h
        src/MathToolkit.h src/MainWindow.cpp src/MainWindow.h src/EQjson.cpp src/EQjson.h
        src/EQList.cpp src/EQList.h src/ShapeEngine.cpp src/ShapeEngine.h src/OptionsWindow.cxx src/OptionsWindow.h src/Parametres.cpp src/Parametres.h src/verticalBox.cpp src/verticalBox.h)

set(CMAKE_CXX_FLAGS_RELEASE -O3)
set(CMAKE_C_FLAGS_RELEASE -O3)

add_executable(FullEarth ${SOURCES})
target_link_libraries(FullEarth ${OPENGL_LIBRARY})
target_link_libraries(FullEarth ${GLEW_LIBRARY})
target_link_libraries(FullEarth ${CURL_LIBRARY})
target_link_libraries(FullEarth ${FLTK_LIBRARIES})

target_include_directories(FullEarth PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(FullEarth PUBLIC ${GLEW_INCLUDE_DIR})
target_include_directories(FullEarth PUBLIC ${CURL_INCLUDE_DIR})
target_include_directories(FullEarth PUBLIC ${FLTK_INCLUDE_DIR})

configure_file(res/FullEarth.desktop ${CMAKE_BINARY_DIR} COPYONLY)