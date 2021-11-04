#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_genalpha(py::module &m)
{
    py::class_<genalpha>(m, "genalpha")
    
        .def(py::init<formulation, vec, vec, int, std::vector<bool>>(), py::arg("formul"), py::arg("dtxinit"), py::arg("dtdtxinit"), py::arg("verbosity")=3, py::arg("isrhskcconstant") = std::vector<bool>{false, false, false, false})

        .def("setverbosity", &genalpha::setverbosity, py::arg("verbosity"))

        .def("setparameter", static_cast<void (genalpha::*)(double, double, double, double)>(&genalpha::setparameter), py::arg("b"), py::arg("g"), py::arg("af"), py::arg("am"))
        .def("setparameter", static_cast<void (genalpha::*)(double)>(&genalpha::setparameter), py::arg("rinf"))

        .def("settolerance", &genalpha::settolerance, py::arg("tol"))

        .def("gettimederivative", &genalpha::gettimederivative)
        .def("settimederivative", &genalpha::settimederivative, py::arg("sol"))

        .def("settimestep", &genalpha::settimestep, py::arg("timestep"))
        .def("gettimestep", &genalpha::gettimestep)

        .def("count", &genalpha::count)
        .def("gettimes", &genalpha::gettimes)

        .def("setadaptivity", &genalpha::setadaptivity, py::arg("tol"), py::arg("mints"), py::arg("maxts"), py::arg("reffact")=0.5, py::arg("coarfact")=2.0, py::arg("coarthres")=0.5)

        .def("presolve", &genalpha::presolve, py::arg("formuls"))
        .def("postsolve", &genalpha::postsolve, py::arg("formuls"))

        .def("next", static_cast<void (genalpha::*)(double)>(&genalpha::next), py::arg("timestep"))
        .def("next", static_cast<int (genalpha::*)(double, int)>(&genalpha::next), py::arg("timestep"), py::arg("maxnumnlit"))
        
    ;
}

