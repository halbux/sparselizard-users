#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_mesh(py::module &m)
{
    py::class_<mesh>(m, "mesh")
    
        .def(py::init<>())
        .def(py::init<std::string, int>(), py::arg("name"), py::arg("verbosity")=1)
        .def(py::init<std::string, int, int, int>(), py::arg("name"), py::arg("globalgeometryskin"), py::arg("numoverlaplayers"), py::arg("verbosity")=1)
        .def(py::init<bool, std::vector<std::string>, int>(), py::arg("mergeduplicates"), py::arg("meshfiles"), py::arg("verbosity")=1)
        .def(py::init<std::vector<shape>, int>(), py::arg("inputshapes"), py::arg("verbosity")=1)
        .def(py::init<std::vector<shape>, int, int, int>(), py::arg("inputshapes"), py::arg("globalgeometryskin"), py::arg("numoverlaplayers"), py::arg("verbosity")=1)

        .def("load", static_cast<void (mesh::*)(std::string, int)>(&mesh::load), py::arg("name"), py::arg("verbosity")=1)
        .def("load", static_cast<void (mesh::*)(std::string, int, int, int)>(&mesh::load), py::arg("name"), py::arg("globalgeometryskin"), py::arg("numoverlaplayers"), py::arg("verbosity")=1)
        .def("load", static_cast<void (mesh::*)(bool, std::vector<std::string>, int)>(&mesh::load), py::arg("mergeduplicates"), py::arg("meshfiles"), py::arg("verbosity")=1)
        .def("load", static_cast<void (mesh::*)(std::vector<shape>, int)>(&mesh::load), py::arg("inputshapes"), py::arg("verbosity")=1)
        .def("load", static_cast<void (mesh::*)(std::vector<shape>, int, int, int)>(&mesh::load), py::arg("inputshapes"), py::arg("globalgeometryskin"), py::arg("numoverlaplayers"), py::arg("verbosity")=1)

        .def("write", static_cast<void (mesh::*)(int, std::string)>(&mesh::write), py::arg("physreg"), py::arg("name"))
        .def("write", static_cast<void (mesh::*)(std::string, std::vector<int>, int)>(&mesh::write), py::arg("name"), py::arg("physregs")=std::vector<int>{-1}, py::arg("option")=1)


        .def("setadaptivity", &mesh::setadaptivity, py::arg("criterion"), py::arg("lownumsplits"), py::arg("highnumsplits"))

        .def("split", &mesh::split, py::arg("n")=1)

        .def("move", static_cast<void (mesh::*)(int, expression)>(&mesh::move), py::arg("physreg"), py::arg("u"))
        .def("move", static_cast<void (mesh::*)(expression)>(&mesh::move), py::arg("u"))

        .def("shift", static_cast<void (mesh::*)(int, double, double, double)>(&mesh::shift), py::arg("physreg"), py::arg("x"), py::arg("y"), py::arg("z"))
        .def("shift", static_cast<void (mesh::*)(double, double, double)>(&mesh::shift), py::arg("x"), py::arg("y"), py::arg("z"))

        .def("rotate", static_cast<void (mesh::*)(int, double, double, double)>(&mesh::rotate), py::arg("physreg"), py::arg("x"), py::arg("y"), py::arg("z"))
        .def("rotate", static_cast<void (mesh::*)(double, double, double)>(&mesh::rotate), py::arg("x"), py::arg("y"), py::arg("z"))

        .def("scale", static_cast<void (mesh::*)(int, double, double, double)>(&mesh::scale), py::arg("physreg"), py::arg("x"), py::arg("y"), py::arg("z"))
        .def("scale", static_cast<void (mesh::*)(double, double, double)>(&mesh::scale), py::arg("x"), py::arg("y"), py::arg("z"))

        .def("getdimension", &mesh::getdimension)
        
        .def("getdimensions", &mesh::getdimensions)
        .def("printdimensions", &mesh::printdimensions)

        .def("getphysicalregionnumbers", &mesh::getphysicalregionnumbers, py::arg("dim")=-1)

        .def("selectskin", static_cast<void (mesh::*)(int, int)>(&mesh::selectskin), py::arg("newphysreg"), py::arg("physregtoskin"))
        .def("selectskin", static_cast<void (mesh::*)(int)>(&mesh::selectskin), py::arg("newphysreg"))

        .def("selectbox", static_cast<void (mesh::*)(int, int, int, std::vector<double>)>(&mesh::selectbox), py::arg("newphysreg"), py::arg("physregtobox"), py::arg("selecteddim"), py::arg("boxlimit"))
        .def("selectbox", static_cast<void (mesh::*)(int, int, std::vector<double>)>(&mesh::selectbox), py::arg("newphysreg"), py::arg("selecteddim"), py::arg("boxlimit"))

        .def("selectsphere", static_cast<void (mesh::*)(int, int, int, std::vector<double>, double)>(&mesh::selectsphere), py::arg("newphysreg"), py::arg("physregtosphere"), py::arg("selecteddim"), py::arg("centercoords"), py::arg("radius"))
        .def("selectsphere", static_cast<void (mesh::*)(int, int, std::vector<double>, double)>(&mesh::selectsphere), py::arg("newphysreg"), py::arg("selecteddim"), py::arg("centercoords"), py::arg("radius"))

        .def("selectlayer", static_cast<void (mesh::*)(int, int, int, int)>(&mesh::selectlayer), py::arg("newphysreg"), py::arg("physregtoselectfrom"), py::arg("physregtostartgrowth"), py::arg("numlayers"))
        .def("selectlayer", static_cast<void (mesh::*)(int, int, int)>(&mesh::selectlayer), py::arg("newphysreg"), py::arg("physregtostartgrowth"), py::arg("numlayers"))

        .def("selectexclusion", static_cast<void (mesh::*)(int, int, std::vector<int>)>(&mesh::selectexclusion), py::arg("newphysreg"), py::arg("physregtoexcludefrom"), py::arg("physregstoexclude"))
        .def("selectexclusion", static_cast<void (mesh::*)(int, std::vector<int>)>(&mesh::selectexclusion), py::arg("newphysreg"), py::arg("physregstoexclude"))

        .def("selectanynode", static_cast<void (mesh::*)(int, int)>(&mesh::selectanynode), py::arg("newphysreg"), py::arg("physregtoselectfrom"))
        .def("selectanynode", static_cast<void (mesh::*)(int)>(&mesh::selectanynode), py::arg("newphysreg"))

        .def("use", &mesh::use)
        
    ;
}

