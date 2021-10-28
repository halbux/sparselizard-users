#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_indexmat(py::module &m)
{
    py::class_<indexmat>(m, "indexmat")
        .def(py::init<>())
        .def(py::init<int, int>(), py::arg("numberofrows"), py::arg("numberofcolumns"))
        .def(py::init<int, int, int>(), py::arg("numberofrows"), py::arg("numberofcolumns"), py::arg("initvalue"))
        .def(py::init<int, int, std::vector<int>>(), py::arg("numberofrows"), py::arg("numberofcolumns"), py::arg("valvec"))
        .def(py::init<int, int, int, int>(), py::arg("numberofrows"), py::arg("numberofcolumns"), py::arg("init"), py::arg("step"))
        .def(py::init<std::vector<indexmat>>(), py::arg("input"))

        .def("countrows", &indexmat::countrows)
        .def("countcolumns", &indexmat::countcolumns)

        .def("count", &indexmat::count)

        .def("print", &indexmat::print)
        .def("printsize", &indexmat::printsize)
    ;
}

