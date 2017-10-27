# Version check
cmake_minimum_required (VERSION 3.0)

# Project
set(mpimatrix "MpiMatrixTest")
project(mpimatrix)

# Globber
file(GLOB sources "*.cpp" "matrix/*.cpp")

add_executable(${mpimatrix} ${sources})


# MPI
find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

# Linking
target_link_libraries(${mpimatrix} ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
  set_target_properties(${mpimatrix} PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(${mpimatrix} PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

target_compile_options(${mpimatrix} PUBLIC -fexceptions -std=c++14 -O3 -Wall -pedantic-errors -pedantic)

