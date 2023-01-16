cmake_minimum_required(VERSION 3.25.1)

include(FetchContent)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fmodules-ts -Werror -Wextra -Wno-attributes]")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

project(
  nathans_minecraft_seed_finder
  VERSION       1.0
  LANGUAGES     CXX
)

FetchContent_Declare(
    googletest
    SOURCE_DIR        "external/googletest"
    GIT_REPOSITORY    https://github.com/google/googletest.git
    GIT_TAG           58d77fa8070e8cec2dc1ed015d66b454c8d78850 # release-1.12.1
)

set(CUBIOMES_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/cubiomes")

ExternalProject_Add(
    cubiomes_src
    SOURCE_DIR          "${CUBIOMES_SRC_DIR}"
    GIT_REPOSITORY      https://github.com/Cubitect/cubiomes.git
    GIT_TAG             c2442e0c9403a18ffcaa96bd6fab46993df0e6d8
    CONFIGURE_COMMAND   ""
    BUILD_COMMAND       make
    BUILD_IN_SOURCE     true
    INSTALL_COMMAND     ""
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(cubiomes STATIC IMPORTED)
set_target_properties(cubiomes PROPERTIES IMPORTED_LOCATION "${CUBIOMES_SRC_DIR}/libcubiomes.a")

#find_package(OpenGL)
find_package(Boost)

include_directories("${CUBIOMES_SRC_DIR}")
add_subdirectory(src)
add_subdirectory(test)

