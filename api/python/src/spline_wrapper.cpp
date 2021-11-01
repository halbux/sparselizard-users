#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_spline(py::module &m)
{
    py::class_<spline>(m, "spline")
    
        .def(py::init<>())
        .def(py::init<std::string, char>(), py::arg("filename"), py::arg("delimiter")='\n')
        .def(py::init<std::vector<double>, std::vector<double>>(), py::arg("xin"), py::arg("yin"))

        .def("set", &spline::set, py::arg("xin"), py::arg("yin"))
        
        .def("getderivative", &spline::getderivative)
        
        .def("getxmin", &spline::getxmin)
        .def("getxmax", &spline::getxmax)
        
        .def("evalat", static_cast<double (spline::*)(double)>(&spline::evalat), py::arg("input"))
        .def("evalat", static_cast<std::vector<double> (spline::*)(std::vector<double>)>(&spline::evalat), py::arg("input"))
        .def("evalat", static_cast<densemat (spline::*)(densemat)>(&spline::evalat), py::arg("input"))
        
        .def("write", &spline::write, py::arg("filename"), py::arg("numsplits"), py::arg("delimiter")='\n')
        
    ;
}

