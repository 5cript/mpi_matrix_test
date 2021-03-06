# Version check
cmake_minimum_required (VERSION 3.0)
cmake_policy(SET CMP0060 NEW)

# Project
set(mpimatrix "MpiMatrixTest")
project(mpimatrix)

option(TASK_3_4 "Enable Task 3_4" ON)
option(TASK_4_2 "Enable Task 4_2" ON)
option(TASK_4_3 "Enable Task 4_3" ON)
option(TASK_4_4 "Enable Task 4_4" ON)
option(TASK_FINAL "Enable Task Final" ON)
option(TRAFFIC_BENCH "Enable Traffic Benchmark" ON)
option(STRIPE_BENCH "Enable Stripe Benchmark" ON)

# Globber
file(GLOB sources 
	"*.cpp" 
	"matrix/*.cpp"
	"bench/*.cpp"
	"mpi/*.cpp"
	"exercises/*.cpp"
)

# MPI
find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

# Boost
set(BOOST_DIR, "$ENV{BOOST_ROOT}")
find_package(Boost 1.61.0 COMPONENTS program_options)

# Include Dirs
include_directories(${Boost_INCLUDE_DIR})

# Linking & Compile Options
link_directories(${Boost_LIBRARY_DIRS})
link_libraries(${MPI_LIBRARIES} ${Boost_LIBRARIES})

function(ALL_TARGET_PROPS target_name)
	if(MPI_LINK_FLAGS)
	  set_target_properties(${target_name} PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}")
	endif()

	if(MPI_COMPILE_FLAGS)
	  set_target_properties(${target_name} PROPERTIES COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
	endif()	
	
	target_compile_options(${target_name} PUBLIC -fexceptions -std=c++14 -O3 -Wall -pedantic-errors -pedantic)
endfunction(ALL_TARGET_PROPS)

if(TASK_3_4)
	add_executable("3_4" ${sources})
	target_compile_definitions("3_4" PRIVATE DO_3_4=1)
endif()
if(TASK_4_2)
	add_executable("4_2" ${sources})
	target_compile_definitions("4_2" PRIVATE DO_4_2=1)
endif()
if(TASK_4_3)
	add_executable("4_3" ${sources})
	target_compile_definitions("4_3" PRIVATE DO_4_3=1)
endif()
if(TASK_4_4)
	add_executable("4_4" ${sources})
	target_compile_definitions("4_4" PRIVATE DO_4_4=1)
endif()
if(TRAFFIC_BENCH)
	add_executable("traffic" ${sources})
	target_compile_definitions("traffic" PRIVATE TRAFFIC_BENCH=1)
endif()
if (TASK_FINAL)
	add_executable("final" ${sources})
	target_compile_definitions("final" PRIVATE FINAL=1)
endif()
if (STRIPE_BENCH)
	add_executable("stripe" ${sources})
	target_compile_definitions("stripe" PRIVATE STRIPE_BENCH=1)
endif()

if(TASK_3_4)
	ALL_TARGET_PROPS("3_4")
endif()
if(TASK_4_2)
	ALL_TARGET_PROPS("4_2")
endif()
if(TASK_4_3)
	ALL_TARGET_PROPS("4_3")
endif()
if(TASK_4_4)
	ALL_TARGET_PROPS("4_4")
endif()
if(TRAFFIC_BENCH)
	ALL_TARGET_PROPS("traffic")
endif()
if (TASK_FINAL)
	ALL_TARGET_PROPS("final")
endif()
if (STRIPE_BENCH)
	ALL_TARGET_PROPS("stripe")
endif()
