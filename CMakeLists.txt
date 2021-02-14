cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(lattice LANGUAGES CXX)

set(CMAKE_SYSTEM_PROCESSOR arm)
set(triple arm64-apple-macos11)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/homebrew/include)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(Qt5_DIR /opt/homebrew/opt/qt/lib/cmake/Qt5)

file(GLOB SOURCES *.cc)

include_directories(/opt/homebrew/opt/qt/include)
link_directories(/opt/homebrew/opt/qt/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Gui Qt5::OpenGL Qt5::Core)