cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(lattice LANGUAGES CXX)

# Neural Network Preprocessor Directive
OPTION(WITH_NEURAL_NETWORK "Neural Network calculator" OFF)

set(CMAKE_SYSTEM_PROCESSOR arm)
set(triple arm64-apple-macos11)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/homebrew/include)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# QT5 Cmake Directory
set(Qt5_DIR /opt/homebrew/opt/qt/lib/cmake/Qt5)

file(GLOB HEADERS include/lattice/*.h)

# Apple Silicon Brew Mac-Hacks
include_directories(/opt/homebrew/opt/assimp/include)
link_directories(/opt/homebrew/opt/assimp/lib)
include_directories(/opt/homebrew/opt/qt/include)
link_directories(/opt/homebrew/opt/qt/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(ASSIMP REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Charts REQUIRED)
find_package(Qt5 COMPONENTS Quick REQUIRED)

IF(WITH_NEURAL_NETWORK)
    add_definitions(-DUSE_NEURAL_NETWORK)
ENDIF(WITH_NEURAL_NETWORK)

add_executable(
        ${PROJECT_NAME}
        src/camera.cc
        src/colors.cc
        src/configuration.cc
        src/configuration_widget.cc
        src/gl_widget.cc
        src/gl_window.cc
        src/keyboard.cc
        src/main.cc
        src/main_window.cc
        src/mass.cc
        src/mass_spring_system.cc
        src/numerics.cc
        src/obj_loader.cc
        src/socket.cc
        src/spring.cc
        src/stats.cc
        src/supervisor.cc

        include/lattice/camera.h
        include/lattice/colors.h
        include/lattice/configuration.h
        include/lattice/configuration_widget.h
        include/lattice/gl_widget.h
        include/lattice/gl_window.h
        include/lattice/keyboard.h
        include/lattice/main_window.h
        include/lattice/mass.h
        include/lattice/mass_spring_system.h
        include/lattice/obj_loader.h
        include/lattice/socket.h
        include/lattice/spring.h
        include/lattice/stats.h
        include/lattice/supervisor.h
        include/lattice/toolbox/numerics.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(
  ${PROJECT_NAME}
  Qt5::Widgets
  Qt5::Gui
  Qt5::OpenGL
  Qt5::Core
  Qt5::Charts
  Qt5::Quick
  Eigen3::Eigen
  assimp
)
