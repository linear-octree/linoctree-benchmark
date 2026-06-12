# ---  LIBRARIES  --- #
# ------------------- #

# Add cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# Find required packages

find_package(OpenMP REQUIRED)
find_package(Eigen3 REQUIRED)


# LASLIB
find_package(LASLIB REQUIRED)
if (${LASLIB_FOUND})
    include_directories(${LASLIB_INCLUDE_DIR} ${LASZIP_INCLUDE_DIR})
    message(STATUS "LASlib include: ${LASLIB_INCLUDE_DIR} ${LASZIP_INCLUDE_DIR}")
else ()
    message(SEND_ERROR "Could not find LASLIB")
endif ()

# Papi
find_package(Papi REQUIRED)
if (PAPI_FOUND)
    message(STATUS "PAPI found and to be linked")
    include_directories(${PAPI_INCLUDE_DIRS})
else ()
    message(SEND_ERROR "Could not find PAPI")
endif ()

# Picotree
find_package(Picotree REQUIRED)
if (PICOTREE_FOUND)
    message(STATUS "Picotree found and to be linked")
    include_directories(${Picotree_INCLUDE_DIRS})
else ()
    message(SEND_ERROR "Could not find Picotree")
endif ()

# PCL
find_package(PCL REQUIRED)
if (PCL_FOUND)
    message(STATUS "PCL found and to be linked")
    include_directories(${PCL_INCLUDE_DIRS})
else ()
    message(SEND_ERROR "Could not find PCL")
endif ()

# yaml-cpp
find_package(yaml-cpp REQUIRED)
if (yaml-cpp_FOUND)
    message(STATUS "yaml-cpp found and to be linked")
    # yaml-cpp < 0.8 (e.g. Ubuntu 22.04) does not export yaml-cpp::yaml-cpp
    if (NOT TARGET yaml-cpp::yaml-cpp AND DEFINED YAML_CPP_LIBRARIES)
        add_library(yaml-cpp::yaml-cpp INTERFACE IMPORTED)
        target_link_libraries(yaml-cpp::yaml-cpp INTERFACE ${YAML_CPP_LIBRARIES})
        target_include_directories(yaml-cpp::yaml-cpp INTERFACE ${YAML_CPP_INCLUDE_DIR})
    endif ()
else ()
    message(SEND_ERROR "Could not find yaml-cpp")
endif ()

# LinOctree
find_package(LinOctree REQUIRED)
if (LinOctree_FOUND)
    message(STATUS "LinOctree found and to be linked")
    include_directories(${LinOctree_INCLUDE_DIRS})
else ()
    message(SEND_ERROR "Could not find LinOctree")
endif ()