#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_port(py::module &m)
{
    //py::class_<port, std::shared_ptr<port> >(m, "port")
    py::class_<port>(m, "port")
        .def(py::init<>())
        .def(py::init<std::vector<int>>(), py::arg("harmonicnumbers"))

        .def("setvalue", &port::setvalue, py::arg("portval"))
        .def("getvalue", &port::getvalue)

        .def("setname", &port::setname, py::arg("name"))
        .def("getname", &port::getname)

        .def("getharmonics", &port::getharmonics)
        
        .def("harmonic", static_cast<port (port::*)(int)>(&port::harmonic), py::arg("harmonicnumber"))
        .def("harmonic", static_cast<port (port::*)(std::vector<int>)>(&port::harmonic), py::arg("harmonicnumber"))

        .def("sin", &port::sin, py::arg("freqindex"))
        .def("cos", &port::cos, py::arg("freqindex"))

        .def("print", &port::print)

        .def("__neg__", static_cast<expression (port::*)()>(&port::operator-))

        // operator (port,port)
        .def("__add__", [](port &a, port &b) { return expression(a) + b;}, py::is_operator())
        .def("__sub__", [](port &a, port &b) { return expression(a) - b;}, py::is_operator())
        .def("__mul__", [](port &a, port &b) { return expression(a) * b;}, py::is_operator())
        .def("__truediv__", [](port &a, port &b) { return expression(a) / b;}, py::is_operator())

        // operator (port, double)
        .def("__add__", [](port &a, double b) { return expression(a) + b;}, py::is_operator())
        .def("__sub__", [](port &a, double b) { return  expression(a) - b;}, py::is_operator())
        .def("__mul__", [](port &a, double b) { return expression(a)*b;}, py::is_operator())
        .def("__truediv__", [](port &a, double b) { return expression(a)/b;}, py::is_operator())

        // operator (double,port)
        .def("__radd__", [](port &a, double b) { return expression(b) + a;}, py::is_operator())
        .def("__rsub__", [](port &a, double b) { return  expression(b) - a;}, py::is_operator())
        .def("__rmul__", [](port &a, double b) { return expression(b)*a;}, py::is_operator())
        .def("__rtruediv__", [](port &a, double b) { return expression(b)/a;}, py::is_operator())
    ;
}
