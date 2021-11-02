#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_mat(py::module &m)
{
    py::class_<mat>(m, "mat")
    
        .def(py::init<>())
        .def(py::init<int, indexmat, indexmat, densemat>(), py::arg("matsize"), py::arg("rowadresses"), py::arg("coladresses"), py::arg("vals"))
        .def(py::init<formulation, indexmat, indexmat, densemat>(), py::arg("myformulation"), py::arg("rowadresses"), py::arg("coladresses"), py::arg("vals"))
        
        .def("countrows", &mat::countrows)
        .def("countcolumns", &mat::countcolumns)

        .def("countnnz", &mat::countnnz)

        .def("reusefactorization", &mat::reusefactorization)
        
        .def("getainds", &mat::getainds)
        .def("getdinds", &mat::getdinds)

        .def("print", &mat::print)
        
        .def("copy", &mat::copy)
        
        
        .def("__pos__", static_cast<mat (mat::*)()>(&mat::operator+))
        .def("__neg__", static_cast<mat (mat::*)()>(&mat::operator-))
        
        // operator (mat, double)
        .def("__mul__", [](mat &a, double &b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](mat &a, double &b) { return a/b;}, py::is_operator())
        
        // operator (mat, mat)
        .def("__mul__", [](mat &a, mat &b) { return a*b;}, py::is_operator())
        .def("__add__", [](mat &a, mat &b) { return a+b;}, py::is_operator())
        .def("__sub__", [](mat &a, mat &b) { return a-b;}, py::is_operator())
        
        // operator (mat, vec)
        .def("__mul__", [](mat &a, vec &b) { return a*b;}, py::is_operator())
        
        // operator (double, mat)
        .def("__rmul__", [](mat &a, double &b) { return b*a;}, py::is_operator())
        
    ;
}

