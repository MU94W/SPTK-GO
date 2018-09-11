# Find the CBlas libraries
#
# It will search MKL, atlas, OpenBlas, reference-cblas in order.
#
# If any cblas implementation found, the following variable will be set.
#    CBLAS_PROVIDER  # one of MKL, ATLAS, OPENBLAS, REFERENCE
#    CBLAS_INC_DIR   # the include directory for cblas.
#    CBLAS_LIBS      # a list of libraries should be linked by paddle. 
#                    # Each library should be full path to object file.
#
# User should set one of MKL_ROOT, ATLAS_ROOT, OPENBLAS_ROOT, REFERENCE_CBLAS_ROOT
# during cmake. If none of them set, it will try to find cblas implementation in
# system paths.
#


## Find MKL First.
# set(MKL_ROOT $ENV{MKL_ROOT} CACHE PATH "Folder contains MKL")
# set(MKL_ROOT /home/hzliudong/mkl/compilers_and_libraries_2016.3.210/linux/mkl CACHE PATH "Folder contains MKL")

#set(MKL_ROOT $ENV{MKL_ROOT} CACHE PATH "Folder contains MKL")
set(MKL_ROOT "/opt/intel/mkl")

find_path(MKL_INCLUDE_DIR mkl.h PATHS ${MKL_ROOT}/include)
find_library(MKL_CORE_LIB 
	NAMES libmkl_core.a mkl_core
	PATHS ${MKL_ROOT}/lib/intel64 ${MKL_ROOT}/lib)
find_library(MKL_LIB 
	NAMES libmkl_sequential.a mkl_sequential
	PATHS ${MKL_ROOT}/lib/intel64 ${MKL_ROOT}/lib)
# find_library(MKL_LIB 
	# NAMES libmkl_tbb_thread.a mkl_tbb_thread
	# PATHS ${MKL_ROOT}/lib/intel64)
# find_library(MKL_LIB 
	# NAMES libmkl_gnu_thread.a mkl_gnu_thread
	# PATHS ${MKL_ROOT}/lib/intel64)
# find_library(MKL_BLAS95_LIB NAMES mkl_blas95_lp64 PATHS ${MKL_ROOT}/lib/intel64)
# find_library(MKL_LAPACK95_LIB NAMES mkl_lapack95_lp64 PATHS ${MKL_ROOT}/lib/intel64)
find_library(MKL_INTEL_C_LIB 
	NAMES libmkl_intel_lp64.a mkl_intel_lp64
	PATHS ${MKL_ROOT}/lib/intel64 ${MKL_ROOT}/lib)
# find_library(MKL_INTEL_LP64 NAMES mkl_intel_lp64 PATHS ${MKL_ROOT}/lib/intel64)

message (STATUS "MKL_ROOT=${MKL_ROOT}")
message (STATUS "MKL_INCLUDE_DIR=${MKL_INCLUDE_DIR}")
message (STATUS "MKL_CORE_LIB=${MKL_CORE_LIB}")
message (STATUS "MKL_SEQUENTIAL_LIB=${MKL_LIB}")
message (STATUS "MKL_INTEL_LP64=${MKL_INTEL_C_LIB}")

# if (MKL_INCLUDE_DIR AND MKL_CORE_LIB AND MKL_SEQUENTIAL_LIB AND MKL_BLAS95_LIB AND MKL_LAPACK95_LIB AND MKL_INTEL_C_LIB )
if (MKL_INCLUDE_DIR AND MKL_CORE_LIB AND MKL_LIB AND MKL_INTEL_C_LIB )
	set(MKL_FOUND ON)
	# set(MKL_INCLUDE_DIRS ${MKL_INCLUDE_DIR})
	set(MKL_LIBRARIES 
	${MKL_CORE_LIB}
	${MKL_LIB}
	${MKL_BLAS95_LIB}
	${MKL_LAPACK95_LIB}
	${MKL_INTEL_C_LIB})
endif()

message (STATUS "MKL_LIBRARIES=${MKL_LIBRARIES}")
