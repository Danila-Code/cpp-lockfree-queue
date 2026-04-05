cmake_minimum_required(VERSION 3.16)

project(
    LockFreeQueue
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

# catch2
find_package(Catch2 REQUIRED)

# includes 
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include_directories(
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)

file(GLOB_RECURSE PROJECT_SOURCES
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

# executables
add_executable(
    LockFreeQueue
    ${PROJECT_SOURCES}
)

target_link_libraries(LockFreeQueue PRIVATE
    Catch2::Catch2WithMain
)

target_include_directories(LockFreeQueue PRIVATE
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)
