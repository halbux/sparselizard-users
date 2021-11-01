#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_shape(py::module &m)
{
    py::class_<shape>(m, "shape")
    
        .def(py::init<>())
        .def(py::init<std::string, int, std::vector<double>>(), py::arg("shapename"), py::arg("physreg"), py::arg("coords"))
        .def(py::init<std::string, int, std::vector<double>, int>(), py::arg("shapename"), py::arg("physreg"), py::arg("coords"), py::arg("nummeshpts"))
        .def(py::init<std::string, int, std::vector<double>, std::vector<int>>(), py::arg("shapename"), py::arg("physreg"), py::arg("coords"), py::arg("nummeshpts"))
        .def(py::init<std::string, int, std::vector<shape>, int>(), py::arg("shapename"), py::arg("physreg"), py::arg("subshapes"), py::arg("nummeshpts"))
        .def(py::init<std::string, int, std::vector<shape>, std::vector<int>>(), py::arg("shapename"), py::arg("physreg"), py::arg("subshapes"), py::arg("nummeshpts"))
        .def(py::init<std::string, int, std::vector<shape>>(), py::arg("shapename"), py::arg("physreg"), py::arg("subshapes"))
        .def(py::init<std::string, int, std::vector<double>, double, int>(), py::arg("shapename"), py::arg("physreg"), py::arg("centercoords"), py::arg("radius"), py::arg("nummeshpts"))
        .def(py::init<std::string, int, shape, double, int>(), py::arg("shapename"), py::arg("physreg"), py::arg("centerpoint"), py::arg("radius"), py::arg("nummeshpts"))

        .def("setphysicalregion", &shape::setphysicalregion, py::arg("physreg"))

        .def("move", &shape::move, py::arg("u"))

        .def("shift", &shape::shift, py::arg("shiftx"), py::arg("shifty"), py::arg("shiftz"))
        .def("scale", &shape::scale, py::arg("scalex"), py::arg("scaley"), py::arg("scalez"))
        .def("rotate", &shape::rotate, py::arg("alphax"), py::arg("alphay"), py::arg("alphaz"))

        .def("extrude", static_cast<shape (shape::*)(int, double, int, std::vector<double>)>(&shape::extrude), py::arg("physreg"), py::arg("height"), py::arg("numlayers"), py::arg("extrudedirection")=std::vector<double>{0,0,1})
        .def("extrude", static_cast<std::vector<shape> (shape::*)(std::vector<int>, std::vector<double>, std::vector<int>, std::vector<double>)>(&shape::extrude), py::arg("physreg"), py::arg("height"), py::arg("numlayers"), py::arg("extrudedirection")=std::vector<double>{0,0,1})

        .def("duplicate", &shape::duplicate)

        .def("getdimension", &shape::getdimension)

        .def("getcoords", &shape::getcoords)

        .def("getname", &shape::getname)
        .def("getsons", &shape::getsons)

        .def("getphysicalregion", &shape::getphysicalregion)
        
    ;
}

