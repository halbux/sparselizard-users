#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_formulation(py::module &m)
{
    py::class_<formulation>(m, "formulation")
    
        .def(py::init<>())

        .def("__iadd__", static_cast<formulation& (formulation::*)(expression)>(&formulation::operator+=), py::arg("expression"))
        .def("__iadd__", static_cast<formulation& (formulation::*)(integration)>(&formulation::operator+=), py::arg("integrationobject"))
        .def("__iadd__", static_cast<formulation& (formulation::*)(std::vector<integration>)>(&formulation::operator+=), py::arg("integrationobject"))

        .def("countdofs", &formulation::countdofs)
        .def("allcountdofs", &formulation::allcountdofs)

        .def("isstiffnessmatrixdefined", &formulation::isstiffnessmatrixdefined)
        .def("isdampingmatrixdefined", &formulation::isdampingmatrixdefined)
        .def("ismassmatrixdefined", &formulation::ismassmatrixdefined)

        .def("generate", static_cast<void (formulation::*)()>(&formulation::generate))
        .def("generatestiffnessmatrix", &formulation::generatestiffnessmatrix)
        .def("generatedampingmatrix", &formulation::generatedampingmatrix)
        .def("generatemassmatrix", &formulation::generatemassmatrix)

        .def("generatein", &formulation::generatein, py::arg("rhskcm"), py::arg("contributionnumbers"))
        .def("generate", static_cast<void (formulation::*)(std::vector<int>)>(&formulation::generate), py::arg("contributionnumbers"))
        .def("generate", static_cast<void (formulation::*)(int)>(&formulation::generate), py::arg("contributionnumber"))

        .def("getportrelations", &formulation::getportrelations)

        .def("b", &formulation::b, py::arg("keepvector")=false, py::arg("dirichletandportupdate")=true)
        .def("A", &formulation::A, py::arg("keepfragments")=false)

        .def("rhs", &formulation::rhs, py::arg("keepvector")=false, py::arg("dirichletandportupdate")=true)
        .def("K", &formulation::K, py::arg("keepfragments")=false)
        .def("C", &formulation::C, py::arg("keepfragments")=false)
        .def("M", &formulation::M, py::arg("keepfragments")=false)

        .def("solve", &formulation::solve, py::arg("soltype")="lu", py::arg("diagscaling")=false, py::arg("blockstoconsider") = std::vector<int>{-1})

        .def("allsolve", static_cast<std::vector<double> (formulation::*)(double, int, std::string, int)>(&formulation::allsolve), py::arg("relrestol"), py::arg("maxnumit"), py::arg("soltype")="lu", py::arg("verbosity")=1)
        .def("allsolve", static_cast<std::vector<double> (formulation::*)(std::vector<int>, std::vector<std::vector<int>>, std::vector<std::vector<int>>, double, int, std::string, int)>(&formulation::allsolve), py::arg("formulterms"), py::arg("physicalterms"), py::arg("artificialterms"), py::arg("relrestol"), py::arg("maxnumit"), py::arg("soltype")="lu", py::arg("verbosity")=1)
        
    ;
}

