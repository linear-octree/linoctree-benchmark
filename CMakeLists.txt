cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(octrees-benchmark-suite LANGUAGES C CXX)

set(PROJECT_VERSION 1.0.0)

# Add cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Status message at configuration time
message(STATUS "")
message(STATUS "Configuring ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Documentation: See INDEX.md or README.md")
message(STATUS "")

message(STATUS "Looking for octrees-benchmark library...")
find_package(LinearOctree REQUIRED)
message(STATUS "Found: ${LinearOctree_INCLUDE_DIR}")

# Gather source files
file(GLOB_RECURSE SUITE_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Find required packages
find_package(OpenMP REQUIRED)
find_package(Eigen3 REQUIRED)

# Compiler flags for SIMD support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native")

# Create executable
add_executable(octrees-benchmark-suite ${SUITE_SOURCES})

# Link the octrees-benchmark library (includes all interface dependencies automatically)
target_link_libraries(octrees-benchmark-suite PRIVATE 
    LinearOctree::LinearOctree
)

# Add suite's own include directory + library headers
target_include_directories(octrees-benchmark-suite PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${LinearOctree_INCLUDE_DIR}
)

# Set C++ standard
set_target_properties(octrees-benchmark-suite PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Optional: Install
install(TARGETS octrees-benchmark-suite RUNTIME DESTINATION bin)

# Status at end of configuration
message(STATUS "")
message(STATUS "Build configured successfully!")
message(STATUS "Run: cmake --build . --parallel $(nproc)")
message(STATUS "")

