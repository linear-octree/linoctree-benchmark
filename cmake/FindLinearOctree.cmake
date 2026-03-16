# FindLinearOctree.cmake
# Módulo para encontrar la librería octrees-benchmark
#
# Variables de entrada:
#   LinearOctree_ROOT_DIR  - Directorio raíz de instalación de octrees-benchmark
#
# Variables de salida:
#   LinearOctree_FOUND         - TRUE si se encontró la librería
#   LinearOctree_INCLUDE_DIR   - Path a los headers
#   LinearOctree_LIBRARIES     - Librerías encontradas
#   LinearOctree_STATIC_LIB    - Librería estática

set(LinearOctree_FOUND FALSE)

# Rutas de búsqueda por defecto
set(_LinearOctree_SEARCH_PATHS
    $ENV{HOME}/.local
    ${LinearOctree_ROOT_DIR}
    /usr/local
    /usr
)

# También buscar relativo al script actual (para desarrollo)
get_filename_component(_module_dir "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
get_filename_component(_project_dir "${_module_dir}/.." ABSOLUTE)
list(INSERT _LinearOctree_SEARCH_PATHS 0 "${_project_dir}/../octrees-benchmark")

# Buscar headers
find_path(LinearOctree_INCLUDE_DIR
    NAMES util.hpp
    HINTS ${_LinearOctree_SEARCH_PATHS}
    PATH_SUFFIXES inc include
)

# Buscar librería estática
find_library(LinearOctree_STATIC_LIB
    NAMES linear-octree_static
    HINTS ${_LinearOctree_SEARCH_PATHS}
    PATH_SUFFIXES lib build lib64
)

# Buscar librería compartida (opcional)
find_library(LinearOctree_SHARED_LIB
    NAMES linear-octree_shared
    HINTS ${_LinearOctree_SEARCH_PATHS}
    PATH_SUFFIXES lib build lib64
)

# Determinar si se encontró la librería
if(LinearOctree_INCLUDE_DIR AND LinearOctree_STATIC_LIB)
    set(LinearOctree_FOUND TRUE)
    set(LinearOctree_LIBRARIES ${LinearOctree_STATIC_LIB})
elseif(LinearOctree_INCLUDE_DIR AND LinearOctree_SHARED_LIB)
    set(LinearOctree_FOUND TRUE)
    set(LinearOctree_LIBRARIES ${LinearOctree_SHARED_LIB})
endif()

# Crear target importado
if(LinearOctree_FOUND AND NOT TARGET LinearOctree::LinearOctree)
    add_library(LinearOctree::LinearOctree STATIC IMPORTED)
    
    # Crear lista de include directories
    set(_interface_includes "${LinearOctree_INCLUDE_DIR}")
    
    # Determinar el directorio padre del proyecto
    # Si LinearOctree_INCLUDE_DIR es /project/inc, _project_root será /project
    # Si LinearOctree_INCLUDE_DIR es /project/include, _project_root será /project
    get_filename_component(_project_root "${LinearOctree_INCLUDE_DIR}" DIRECTORY)
    
    # Agregar paths de dependencias (buscando en project/lib/XXX)
    if(EXISTS "${_project_root}/lib/eigen3/include/eigen3")
        list(APPEND _interface_includes "${_project_root}/lib/eigen3/include/eigen3")
    endif()
    
    if(EXISTS "${_project_root}/lib/LASlib/include/LASlib")
        list(APPEND _interface_includes "${_project_root}/lib/LASlib/include/LASlib")
    endif()
    
    if(EXISTS "${_project_root}/lib/papi/include")
        list(APPEND _interface_includes "${_project_root}/lib/papi/include")
    endif()
    
    set_target_properties(LinearOctree::LinearOctree PROPERTIES
        IMPORTED_LOCATION "${LinearOctree_STATIC_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${_interface_includes}"
    )
    
    # Linkear con OpenMP
    find_package(OpenMP QUIET)
    if(OpenMP_CXX_FOUND)
        set_property(TARGET LinearOctree::LinearOctree 
            APPEND PROPERTY INTERFACE_LINK_LIBRARIES OpenMP::OpenMP_CXX)
    endif()
    
    # Linkear con Eigen3
    find_package(Eigen3 QUIET)
    if(Eigen3_FOUND)
        set_property(TARGET LinearOctree::LinearOctree 
            APPEND PROPERTY INTERFACE_LINK_LIBRARIES Eigen3::Eigen)
    endif()
    
    # Linkear con dependencias del directorio lib (búsqueda en proyecto)
    if(EXISTS "${_project_root}/lib/LASlib/lib/LASlib/libLASlib.a")
        set_property(TARGET LinearOctree::LinearOctree 
            APPEND PROPERTY INTERFACE_LINK_LIBRARIES "${_project_root}/lib/LASlib/lib/LASlib/libLASlib.a")
    endif()
    
    if(EXISTS "${_project_root}/lib/papi/lib/libpapi.so")
        set_property(TARGET LinearOctree::LinearOctree 
            APPEND PROPERTY INTERFACE_LINK_LIBRARIES "${_project_root}/lib/papi/lib/libpapi.so")
    endif()
endif()

# Mensaje de estado
if(NOT LinearOctree_FIND_QUIETLY)
    if(LinearOctree_FOUND)
        message(STATUS "Found LinearOctree: ${LinearOctree_STATIC_LIB}")
        message(STATUS "LinearOctree include: ${LinearOctree_INCLUDE_DIR}")
        message(DEBUG "LinearOctree _lib_root: ${_lib_root}")
        message(DEBUG "LinearOctree _project_root: ${_project_root}")
        message(DEBUG "LinearOctree _interface_includes: ${_interface_includes}")
        message(STATUS "PAPI include path check: ${_lib_root}/papi/include = ${EXISTS_${_lib_root}/papi/include}")
        if(EXISTS "${_lib_root}/papi/include")
            message(STATUS "✓ Found PAPI includes at: ${_lib_root}/papi/include")
        elseif(EXISTS "${_project_root}/lib/papi/include")
            message(STATUS "✓ Found PAPI includes at: ${_project_root}/lib/papi/include")
        else()
            message(STATUS "✗ PAPI includes NOT found")
        endif()
    else()
        message(STATUS "LinearOctree not found")
        if(LinearOctree_FIND_REQUIRED)
            message(FATAL_ERROR 
                "Could not find LinearOctree. "
                "Set LinearOctree_ROOT_DIR or ensure it's installed in standard paths like /usr/local or /usr"
            )
        endif()
    endif()
endif()

# Marcar variables como avanzadas
mark_as_advanced(
    LinearOctree_INCLUDE_DIR
    LinearOctree_STATIC_LIB
    LinearOctree_SHARED_LIB
    LinearOctree_LIBRARIES
)
