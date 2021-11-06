#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_parameter(py::module &m)
{
    py::class_<parameter>(m, "parameter")
    
        .def(py::init<>())
        .def(py::init<int, int>(), py::arg("numrows"), py::arg("numcols"))

        .def("print", &parameter::print)

        .def("countrows", &parameter::countrows)
        .def("countcolumns", &parameter::countcolumns)

        .def("setvalue", &parameter::setvalue, py::arg("physreg"), py::arg("input"))
        
        .def("atbarycenter", &parameter::atbarycenter, py::arg("physreg"), py::arg("onefield"))

        .def("max", static_cast<std::vector<double> (parameter::*)(int, int, std::vector<double>)>(&parameter::max), py::arg("physreg"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})
        .def("max", static_cast<std::vector<double> (parameter::*)(int, expression, int, std::vector<double>)>(&parameter::max), py::arg("physreg"), py::arg("meshdeform"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})
        .def("min", static_cast<std::vector<double> (parameter::*)(int, int, std::vector<double>)>(&parameter::min), py::arg("physreg"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})
        .def("min", static_cast<std::vector<double> (parameter::*)(int, expression, int, std::vector<double>)>(&parameter::min), py::arg("physreg"), py::arg("meshdeform"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})

        .def("interpolate", static_cast<void (parameter::*)(int, std::vector<double>&, std::vector<double>&, std::vector<bool>&)>(&parameter::interpolate), py::arg("physreg"), py::arg("xyzcoord"), py::arg("interpolated"), py::arg("isfound"))
        .def("interpolate", static_cast<void (parameter::*)(int, expression, std::vector<double>&, std::vector<double>&, std::vector<bool>&)>(&parameter::interpolate), py::arg("physreg"), py::arg("meshdeform"), py::arg("xyzcoord"), py::arg("interpolated"), py::arg("isfound"))
        .def("interpolate", static_cast<std::vector<double> (parameter::*)(int, const std::vector<double>)>(&parameter::interpolate), py::arg("physreg"), py::arg("xyzcoord"))
        .def("interpolate", static_cast<std::vector<double> (parameter::*)(int, expression, const std::vector<double>)>(&parameter::interpolate), py::arg("physreg"), py::arg("meshdeform"), py::arg("xyzcoord"))

        .def("integrate", static_cast<double (parameter::*)(int, int)>(&parameter::integrate), py::arg("physreg"), py::arg("integrationorder"))
        .def("integrate", static_cast<double (parameter::*)(int, expression, int)>(&parameter::integrate), py::arg("physreg"), py::arg("meshdeform"), py::arg("integrationorder"))

        .def("write", static_cast<void (parameter::*)(int, int, std::string, int)>(&parameter::write), py::arg("physreg"), py::arg("numfftharms"), py::arg("filename"), py::arg("lagrangeorder"))
        .def("write", static_cast<void (parameter::*)(int, int, expression, std::string, int)>(&parameter::write), py::arg("physreg"), py::arg("numfftharms"), py::arg("meshdeform"), py::arg("filename"), py::arg("lagrangeorder"))
        .def("write", static_cast<void (parameter::*)(int, std::string, int, int)>(&parameter::write), py::arg("physreg"), py::arg("filename"), py::arg("lagrangeorder"), py::arg("numtimesteps")=-1)
        .def("write", static_cast<void (parameter::*)(int, expression, std::string, int, int)>(&parameter::write), py::arg("physreg"), py::arg("meshdeform"), py::arg("filename"), py::arg("lagrangeorder"), py::arg("numtimesteps")=-1)


        .def("__pos__", static_cast<expression (parameter::*)()>(&parameter::operator+))
        .def("__neg__", static_cast<expression (parameter::*)()>(&parameter::operator-))

        // operator (parameter, parameter)
        .def("__add__", [](parameter &a, parameter &b) { return a+b;}, py::is_operator())
        .def("__sub__", [](parameter &a, parameter &b) { return a-b;}, py::is_operator())
        .def("__mul__", [](parameter &a, parameter &b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](parameter &a, parameter &b) { return a/b;}, py::is_operator())

        // operator (parameter, double)
        .def("__add__", [](parameter &a, double b) { return a+b;}, py::is_operator())
        .def("__sub__", [](parameter &a, double b) { return a-b;}, py::is_operator())
        .def("__mul__", [](parameter &a, double b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](parameter &a, double b) { return a/b;}, py::is_operator())
        
        // operator (double, parameter)
        .def("__radd__", [](parameter &a, double b) { return b+a;}, py::is_operator())
        .def("__rsub__", [](parameter &a, double b) { return b-a;}, py::is_operator())
        .def("__rmul__", [](parameter &a, double b) { return b*a;}, py::is_operator())
        .def("__rtruediv__", [](parameter &a, double b) { return b/a;}, py::is_operator())
        
    ;
}

