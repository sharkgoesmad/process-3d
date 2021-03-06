if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-tree build detected. Aborting ...")
endif()


cmake_minimum_required(VERSION 2.6)

include("cmake/compiler.cmake")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    option(PB_DEBUG ON)
    add_definitions(-DPB_DEBUG)
    set(ENV{CFLAGS} "-O0 -g")
    set(ENV{CXXFLAGS} "$ENV{CFLAGS}")
else()
    add_definitions(-DNDEBUG)
    set(ENV{CFLAGS} "-O3")
    set(ENV{CXXFLAGS} "$ENV{CFLAGS}")
endif()


set(PB_PROJECT Process3D)
project(${PB_PROJECT})


set(PB_ROOT_DIR ${${PB_PROJECT}_SOURCE_DIR})
set(PB_OUTPUT_DIR ${CMAKE_BINARY_DIR})
set(PB_INCLUDE_DIRS "")


find_package(OpenGL REQUIRED)

# build dependencies
add_subdirectory("${PB_ROOT_DIR}/external")


# include directories
list(APPEND PB_INCLUDE_DIRS ${PB_ROOT_DIR}/external/SDL2/include)
list(APPEND PB_INCLUDE_DIRS ${PB_ROOT_DIR}/external/glew/include)
list(APPEND PB_INCLUDE_DIRS ${PB_ROOT_DIR}/external/glm)
list(APPEND PB_INCLUDE_DIRS ${PB_ROOT_DIR}/external/bullet3/src)
list(APPEND PB_INCLUDE_DIRS ${PB_ROOT_DIR}/external/AntTweakBar-sdl2/include)
list(APPEND PB_INCLUDE_DIRS ${PB_ROOT_DIR}/include)
list(APPEND PB_INCLUDE_DIRS ${PB_ROOT_DIR}/src)




# accumulate source files
file(GLOB PB_SOURCE_FILES
    ${PB_ROOT_DIR}/src/*.cpp
    ${PB_ROOT_DIR}/src/core/*.cpp
    ${PB_ROOT_DIR}/src/reas/*.cpp
    )


include_directories(BEFORE ${PB_INCLUDE_DIRS})
link_directories(${PB_OUTPUT_DIR})
add_executable(${PB_PROJECT} ${PB_SOURCE_FILES})
target_link_libraries(${PB_PROJECT}
    SDL2main
    SDL2
    glew
    BulletDynamics
    BulletCollision
    LinearMath
    AntTweakBar
    )



# configuration output
message(STATUS "${PB_PROJECT} configuration:")
message(STATUS "  ROOT_DIR: ${PB_ROOT_DIR}")
message(STATUS "  PB_OUTPUT_DIR: ${PB_OUTPUT_DIR}")
message(STATUS "  PB_INCLUDE_DIRS: ${PB_INCLUDE_DIRS}")

message(STATUS "  CFLAGS: $ENV{CFLAGS}")
message(STATUS "  CXXFLAGS: $ENV{CXXFLAGS}")

