TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += "NOT_LOGGED_IN=1"

LIBS += -pthread -Wl,-rpath -Wl,/usr/lib/openmpi -Wl,--enable-new-dtags -L/usr/lib/openmpi -lmpi -lmpi_cxx -lboost_program_options

SOURCES += main.cpp \
    bench/benchhelp.cpp \
    matrix/block_descriptor.cpp \
    matrix/block_list.cpp \
    matrix/matrix.cpp \
    matrix/matrix_block.cpp \
    matrix/matrix_loader.cpp \
    matrix/matrix_partition.cpp \
    matrix/matrix_vector.cpp \
    matrix/matrix_vector_view.cpp \
    mpi/communicator.cpp \
    mpi/error.cpp \
    mpi/mpi_core.cpp \
    mpi/shared_file.cpp \
    helpers.cpp \
    parallel_matrix.cpp \
    program_options.cpp \
    debug.cpp \
    exercises/3_4.cpp \
    exercises/3_5.cpp \
    exercises/4_2.cpp \
    exercises/4_3.cpp \
    exercises/4_4.cpp

HEADERS += \
    bench/benchhelp.hpp \
    matrix/block_descriptor.hpp \
    matrix/block_list.hpp \
    matrix/forward.hpp \
    matrix/matrix.hpp \
    matrix/matrix_block.hpp \
    matrix/matrix_loader.hpp \
    matrix/matrix_partition.hpp \
    matrix/matrix_value_type.hpp \
    matrix/matrix_vector.hpp \
    matrix/matrix_vector_view.hpp \
    matrix/matrix_view.hpp \
    mpi/communicator.hpp \
    mpi/error.hpp \
    mpi/group.hpp \
    mpi/mpi_core.hpp \
    mpi/shared_file.hpp \
    mpi/types.hpp \
    constants.hpp \
    container_print.hpp \
    helpers.hpp \
    parallel_matrix.hpp \
    program_options.hpp \
    stream_helpers.hpp \
    debug.hpp \
    exercises/3_4.hpp \
    exercises/3_5.hpp \
    exercises/4_2.hpp \
    exercises/4_3.hpp \
    exercises/4_4.hpp
