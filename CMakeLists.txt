cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

project(CmakeTest)

SET( EIGEN3_INCLUDE_DIR "deps/eigen-3.4.0" )
IF( NOT EIGEN3_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
ENDIF()
INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )

add_executable(tetra
  src/tetrahedron.cc
)

target_link_libraries(tetra)