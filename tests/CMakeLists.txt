# Use shared FetchContent cache so GoogleTest is only downloaded once
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/build/_deps_cache" CACHE PATH "FetchContent cache")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

# YAML config test
add_executable(test_yaml_config test_yaml_config.cpp)
target_link_libraries(test_yaml_config PRIVATE suite_lib gtest_main)
gtest_discover_tests(test_yaml_config)
