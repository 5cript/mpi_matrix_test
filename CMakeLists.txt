# Version check
cmake_minimum_required (VERSION 3.0)
cmake_policy(SET CMP0060 NEW)

# Project
set(mpimatrix "MpiMatrixTest")
project(mpimatrix)

# Globber
file(GLOB sources 
	"*.cpp" 
	"matrix/*.cpp"
	"bench/*.cpp"
	"mpi/*.cpp"
)

add_executable(${mpimatrix} ${sources})

# MPI
find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

# Boost
set(BOOST_DIR, "$ENV{BOOST_ROOT}")
find_package(Boost 1.61.0 COMPONENTS program_options)

#add_library(boost_program_options UNKNOWN IMPORTED)
#set_property(TARGET boost_program_options PROPERTY IMPORTED_LOCATION "${Boost_LIBRARY_DIRS}")

# Include Dirs
include_directories(${Boost_INCLUDE_DIR})

# Linking
link_directories(${Boost_LIBRARY_DIRS})
target_link_libraries(${mpimatrix} ${MPI_LIBRARIES} ${Boost_LIBRARIES})

if(MPI_COMPILE_FLAGS)
  set_target_properties(${mpimatrix} PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(${mpimatrix} PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

target_compile_options(${mpimatrix} PUBLIC -fexceptions -std=c++14 -O3 -Wall -pedantic-errors -pedantic)

