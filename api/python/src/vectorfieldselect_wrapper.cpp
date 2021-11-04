#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_vectorfieldselect(py::module &m)
{
    py::class_<vectorfieldselect>(m, "vectorfieldselect")
    
        .def("setdata", &vectorfieldselect::setdata, py::arg("physreg"), py::arg("myfield"), py::arg("op")="set")
        
    ;
}

