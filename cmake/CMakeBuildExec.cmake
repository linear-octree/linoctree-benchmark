# ---  BUILDING  --- #
# ------------------ #

# Status message at configuration time
message(STATUS "")
message(STATUS "Configuring ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Documentation: See INDEX.md or README.md")
message(STATUS "")



# Compiler flags for SIMD support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native")

# Create executable
add_executable(${PROJECT_NAME} ${SUITE_SOURCES})


### LINKING ###

# OpenMP
if (TARGET OpenMP::OpenMP_CXX)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
else()
    message(SEND_ERROR "Could not find OpenMP")
endif ()

# LinOctree
if (TARGET linoctree::linoctree)
    message(STATUS "LinOctree library found and to be linked")
    target_link_libraries(${PROJECT_NAME} PRIVATE linoctree::linoctree)
else()
    message(SEND_ERROR "Could not find LinOctree library")
endif ()

# PCL
if (TARGET PCL::PCL)
    message(STATUS "PCL library found and to be linked")
    target_link_libraries(${PROJECT_NAME} PRIVATE PCL::PCL)
else()
    message(SEND_ERROR "Could not find PCL library")
endif ()

# PAPI
if (PAPI_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PAPI_LIBRARIES})
endif ()

# LASLIB
if (LASLIB_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LASLIB_LIBRARIES})
endif ()

# Add suite's own include directory + library headers
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# Set C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

# Optional: Install
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# Status at end of configuration
message(STATUS "")
message(STATUS "Build configured successfully!")
message(STATUS "Run: cmake --build . --parallel $(nproc)")
message(STATUS "")