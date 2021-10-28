#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_integration(py::module &m)
{
    py::class_<integration>(m, "integration")
    
        .def(py::init<>())
        .def(py::init<int, expression, int, int>(), py::arg("physreg"), py::arg("tointegrate"), py::arg("integrationorderdelta")=0, py::arg("blocknumber")=0)
        .def(py::init<int, expression, expression, int, int>(), py::arg("physreg"), py::arg("meshdeform"), py::arg("tointegrate"), py::arg("integrationorderdelta")=0, py::arg("blocknumber")=0)
        .def(py::init<int, int, expression, int, int>(), py::arg("physreg"), py::arg("numcoefharms"), py::arg("tointegrate"), py::arg("integrationorderdelta")=0, py::arg("blocknumber")=0)
        .def(py::init<int, int, expression, expression, int, int>(), py::arg("physreg"), py::arg("numcoefharms"), py::arg("meshdeform"), py::arg("tointegrate"), py::arg("integrationorderdelta")=0, py::arg("blocknumber")=0)

        .def("getexpression", &integration::getexpression)
        .def("ismeshdeformdefined", &integration::ismeshdeformdefined)
        .def("getmeshdeform", &integration::getmeshdeform)

        .def("getphysicalregion", &integration::getphysicalregion)
        .def("getintegrationorderdelta", &integration::getintegrationorderdelta)
        .def("getblocknumber", &integration::getblocknumber)

        .def("isfftrequested", &integration::isfftrequested)
        .def("getnumberofcoefharms", &integration::getnumberofcoefharms)

        .def("print", &integration::print)
        
    ;
}

