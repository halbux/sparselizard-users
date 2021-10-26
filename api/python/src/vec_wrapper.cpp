#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_vec(py::module &m)
{
    //py::class_<vec, std::shared_ptr<vec> >(m, "vec")
    py::class_<vec>(m, "vec")
        .def(py::init<>())
        .def(py::init<formulation>(), py::arg("formul"))

        .def("size", &vec::size)

        .def("updateconstraints", &vec::updateconstraints)

        .def("setvalue", static_cast<void (vec::*)(int, double, std::string)>(&vec::setvalue), py::arg("address"), py::arg("value"), py::arg("op")="set")
        .def("getvalue", static_cast<double (vec::*)(int)>(&vec::getvalue), py::arg("address"))

        .def("setvalue", static_cast<void (vec::*)(port, double, std::string)>(&vec::setvalue), py::arg("prt"), py::arg("value"), py::arg("op")="set")
        .def("getvalue", static_cast<double (vec::*)(port)>(&vec::getvalue), py::arg("prt"))

        .def("setdata", static_cast<void (vec::*)(int, field, std::string)>(&vec::setdata), py::arg("physreg"), py::arg("myfield"), py::arg("op")="set")
        .def("setdata", static_cast<void (vec::*)()>(&vec::setdata))

        .def("automaticupdate", &vec::automaticupdate, py::arg("updateit"))
        .def("noautomaticupdate", &vec::noautomaticupdate)

        .def("write", &vec::write, py::arg("filename"))
        .def("load", &vec::load, py::arg("filename"))
        .def("print", &vec::print)

        .def("copy", &vec::copy)

        .def("norm", &vec::norm, py::arg("type")="2")
        .def("sum", &vec::sum)
    ;
}

