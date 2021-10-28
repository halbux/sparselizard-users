#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_impliciteuler(py::module &m)
{
    py::class_<impliciteuler>(m, "impliciteuler")
    
        .def(py::init<formulation, vec, int, std::vector<bool>>(), py::arg("formul"), py::arg("dtxinit"), py::arg("verbosity")=3, py::arg("isrhskcconstant")=std::vector<bool>{false, false, false})

        .def("setverbosity", &impliciteuler::setverbosity, py::arg("verbosity"))

        .def("settolerance", &impliciteuler::settolerance, py::arg("tol"))

        .def("setrelaxationfactor", &impliciteuler::setrelaxationfactor, py::arg("relaxfact"))

        .def("gettimederivative", &impliciteuler::gettimederivative)
        .def("settimederivative", &impliciteuler::settimederivative, py::arg("sol"))

        .def("settimestep", &impliciteuler::settimestep, py::arg("timestep"))
        .def("gettimestep", &impliciteuler::gettimestep)

        .def("count", &impliciteuler::count)
        .def("gettimes", &impliciteuler::gettimes)

        .def("setadaptivity", &impliciteuler::setadaptivity, py::arg("tol"), py::arg("mints"), py::arg("maxts"), py::arg("reffact")=0.5, py::arg("coarfact")=2.0, py::arg("coarthres")=0.5)

        .def("presolve", &impliciteuler::presolve, py::arg("formuls"))
        .def("postsolve", &impliciteuler::postsolve, py::arg("formuls"))

        .def("next", static_cast<void (impliciteuler::*)(double)>(&impliciteuler::next), py::arg("timestep"))
        .def("next", static_cast<int (impliciteuler::*)(double, int)>(&impliciteuler::next), py::arg("timestep"), py::arg("maxnumnlit"))
        
    ;
}

