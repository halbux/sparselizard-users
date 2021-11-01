#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_universe(py::module &m)
{
    py::class_<universe>(m, "universe")
    
        .def_readwrite_static("maxnumthreads", &universe::maxnumthreads)
        .def_static("getmaxnumthreads", &universe::getmaxnumthreads)
        .def_static("setmaxnumthreads", &universe::setmaxnumthreads, py::arg("mnt"))

        .def_readwrite_static("roundoffnoiselevel", &universe::roundoffnoiselevel)
        
    ;
}

