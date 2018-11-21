cmake_minimum_required(VERSION 2.8)

PROJECT(SPTK-GO)                                             #定义整个CMake的工程名

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmakeModules/")

find_package(MKL)
if (MKL_FOUND)
    message (STATUS "MKL_INCLUDE_DIR=${MKL_INCLUDE_DIR}")
    message (STATUS "MKL_LIBRARIES=${MKL_LIBRARIES}")
endif()

FILE(GLOB LIBFILES "lib/*.cc")
FILE(GLOB TOOLFILES "bin/*/_*.cc")

SET(HEADERS
    ${PROJECT_SOURCE_DIR}
    ${MKL_INCLUDE_DIR}
    "lib/"
    "bin"
    "bin/mlpg"
    "bin/freqt"
    "bin/upsample"
    "/usr/include"
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -ldl -fPIC")

set(LIBRARIES
    ${MKL_LIBRARIES}
    gomp
    pthread
    -ldl
    )

INCLUDE_DIRECTORIES(${HEADERS})

ADD_EXECUTABLE(mlpgGO "bin/mlpg/mlpgGO.cc" ${LIBFILES} ${TOOLFILES})
TARGET_LINK_LIBRARIES(mlpgGO -ldl -Wl,--start-group ${LIBRARIES} -Wl,--end-group)

ADD_EXECUTABLE(freqtGO "bin/freqt/freqtGO.cc" ${LIBFILES} ${TOOLFILES})
TARGET_LINK_LIBRARIES(freqtGO -ldl -Wl,--start-group ${LIBRARIES} -Wl,--end-group)
