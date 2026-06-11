# --- CONFIGURATION --- #
# --------------------- #

# Setup compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -w ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g ${CMAKE_CXX_FLAGS}")

