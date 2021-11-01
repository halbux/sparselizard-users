#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_mat(py::module &m)
{
    py::class_<mat>(m, "mat")
    
        .def(py::init<>())
        
        .def("isdefined", &mat::isdefined)
        
        .def("countrows", &mat::countrows)
        .def("countcolumns", &mat::countcolumns)

        .def("countnnz", &mat::countnnz)

        .def("reusefactorization", &mat::reusefactorization)

        .def("xbmerge", &mat::xbmerge)
        .def("x0merge", &mat::x0merge)

        .def("eliminate", &mat::eliminate)
        .def("print", &mat::print)
        .def("copy", &mat::copy)
        
    ;
}

