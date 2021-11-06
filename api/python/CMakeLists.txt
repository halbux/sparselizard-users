# Thanks to H. Badi for bringing sparselizard python bindings to the project.

cmake_minimum_required(VERSION 3.10)

project(spylizard LANGUAGES CXX VERSION 1.0 DESCRIPTION "sparselizard python wrapper")
set(CMAKE_CXX_STANDARD 11)

set(DEFAULT_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build" FORCE)

set (CMAKE_CXX_COMPILER "g++")
set (CMAKE_CXX_FLAGS "-fPIC")

set (PYTHON_DIR "/usr/include/python3.9")
set (PYBIND11_DIR "/usr/include/pybind11")

set (SPARSELIZARD_DIR "../../../sparselizard")
set (SPARSELIZARD_SRC "${SPARSELIZARD_DIR}/src")

set (PETSC_DIR "~/SLlibs/petsc")
set (PETSC_ARCH "arch-linux-c-opt")

include_directories(
    ${SPARSELIZARD_SRC}
    ${SPARSELIZARD_SRC}/expression
    ${SPARSELIZARD_SRC}/expression/operation
    ${SPARSELIZARD_SRC}/field
    ${SPARSELIZARD_SRC}/formulation
    ${SPARSELIZARD_SRC}/gausspoint
    ${SPARSELIZARD_SRC}/geometry
    ${SPARSELIZARD_SRC}/io
    ${SPARSELIZARD_SRC}/io/gmsh
    ${SPARSELIZARD_SRC}/io/nastran
    ${SPARSELIZARD_SRC}/io/paraview
    ${SPARSELIZARD_SRC}/mesh
    ${SPARSELIZARD_SRC}/resolution
    ${SPARSELIZARD_SRC}/shapefunction
    ${SPARSELIZARD_SRC}/shapefunction/hierarchical
    ${SPARSELIZARD_SRC}/shapefunction/hierarchical/h1
    ${SPARSELIZARD_SRC}/shapefunction/hierarchical/hcurl
    ${SPARSELIZARD_SRC}/shapefunction/lagrange
)

include_directories(${PETSC_DIR}/include ${PETSC_DIR}/${PETSC_ARCH}/include)

include_directories(
    ${PYTHON_DIR}
    ${PYBIND11_DIR}
)

add_library(spylizard SHARED
    src/densemat_wrapper.cpp
    src/eigenvalue_wrapper.cpp
    src/expression_wrapper.cpp
    src/field_wrapper.cpp
    src/formulation_wrapper.cpp
    src/genalpha_wrapper.cpp
    src/impliciteuler_wrapper.cpp
    src/indexmat_wrapper.cpp
    src/integration_wrapper.cpp
    src/mat_wrapper.cpp
    src/mesh_wrapper.cpp
    src/parameter_wrapper.cpp
    src/port_wrapper.cpp
    src/shape_wrapper.cpp
    src/sl_wrapper.cpp
    src/slmpi_wrapper.cpp
    src/spanningtree_wrapper.cpp
    src/sparselizard_wrapper.cpp
    src/spline_wrapper.cpp
    src/universe_wrapper.cpp
    src/vec_wrapper.cpp
    src/vectorfieldselect_wrapper.cpp
    src/wallclock_wrapper.cpp
)

#find_library(SPARSELIZARD_LIBRARY NAMES libsparselizard.so ${SPARSELIZARD_DIR})

SET_TARGET_PROPERTIES(spylizard PROPERTIES PREFIX "")
target_link_libraries(spylizard sparselizard petsc slepc)
target_link_directories(spylizard PUBLIC ../../../sparselizard/build ${PETSC_DIR}/include ${PETSC_DIR}/${PETSC_ARCH}/lib)

