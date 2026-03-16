# - Find LinOctree
# Find the LinOctree headers and libraries.
#
# This module defines:
#  LinOctree_INCLUDE_DIRS - where to find linoctree headers
#  LinOctree_LIBRARIES    - List of libraries when using LinOctree
#  LinOctree_FOUND        - True if LinOctree found
#
# Targets:
#  linoctree::linoctree   - Imported target for the linoctree library

find_path(LinOctree_INCLUDE_DIR
    NAMES util.hpp
    PATHS ${CMAKE_SOURCE_DIR}/lib/linoctree/include
          ${CMAKE_PREFIX_PATH}
          $ENV{HOME}/.local
    PATH_SUFFIXES include/linoctree include
    DOC "Path to LinOctree include directory"
)

find_library(LinOctree_LIBRARY_SHARED
    NAMES linoctree_shared
    PATHS ${CMAKE_SOURCE_DIR}/lib/linoctree/lib
          ${CMAKE_PREFIX_PATH}
          $ENV{HOME}/.local
    PATH_SUFFIXES lib lib64
    DOC "Path to LinOctree shared library"
)

find_library(LinOctree_LIBRARY_STATIC
    NAMES linoctree_static
    PATHS ${CMAKE_SOURCE_DIR}/lib/linoctree/lib
          ${CMAKE_PREFIX_PATH}
          $ENV{HOME}/.local
    PATH_SUFFIXES lib lib64
    DOC "Path to LinOctree static library"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LinOctree
    REQUIRED_VARS LinOctree_INCLUDE_DIR
    HANDLE_COMPONENTS
)

if(LinOctree_FOUND)
    # Set include directories
    set(LinOctree_INCLUDE_DIRS ${LinOctree_INCLUDE_DIR})

    # Prefer shared library if found, otherwise use static
    if(LinOctree_LIBRARY_SHARED)
        set(LinOctree_LIBRARIES ${LinOctree_LIBRARY_SHARED})
    elseif(LinOctree_LIBRARY_STATIC)
        set(LinOctree_LIBRARIES ${LinOctree_LIBRARY_STATIC})
    endif()

    # Create an imported target for modern CMake usage
    if(NOT TARGET linoctree::linoctree)
        if(LinOctree_LIBRARY_SHARED)
            add_library(linoctree::linoctree SHARED IMPORTED)
            set_target_properties(linoctree::linoctree PROPERTIES
                IMPORTED_LOCATION "${LinOctree_LIBRARY_SHARED}"
            )
        elseif(LinOctree_LIBRARY_STATIC)
            add_library(linoctree::linoctree STATIC IMPORTED)
            set_target_properties(linoctree::linoctree PROPERTIES
                IMPORTED_LOCATION "${LinOctree_LIBRARY_STATIC}"
            )
        endif()

        set_target_properties(linoctree::linoctree PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${LinOctree_INCLUDE_DIRS}"
        )
    endif()
endif()

mark_as_advanced(LinOctree_INCLUDE_DIR LinOctree_LIBRARY_SHARED LinOctree_LIBRARY_STATIC)

