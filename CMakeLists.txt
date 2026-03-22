cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(octrees-benchmark-suite LANGUAGES C CXX)

set(PROJECT_VERSION 1.0.0)

# Configure CMake
message("Configuring CMake ...")
include(cmake/CMakeConfig.cmake)
message("CMake configured!\n")

# Handle sources
message("Handling sources ...")
include(cmake/CMakeSources.cmake)
message("Sources handled!\n")

# Load libraries
message("Loading libraries ...")
include(cmake/CMakeLibraries.cmake)
message("Libraries loaded!\n")

# Build executable
message("Building ${PROJECT_NAME} ...")
include(cmake/CMakeBuildExec.cmake)
message("Project built!\n\n")

# Tests (optional)
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    message("Configuring tests ...")
    add_subdirectory(tests)
    message("Tests configured!\n\n")
endif()
