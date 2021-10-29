#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_eigenvalue(py::module &m)
{
    py::class_<eigenvalue>(m, "eigenvalue")
    
        .def(py::init<mat>(), py::arg("A"))
        .def(py::init<mat, mat>(), py::arg("A"), py::arg("B"))
        .def(py::init<mat, mat, mat>(), py::arg("K"), py::arg("C"), py::arg("M"))
        .def(py::init<std::vector<mat>>(), py::arg("inmats"))

        .def("compute", &eigenvalue::compute, py::arg("numeigenvaluestocompute"), py::arg("targeteigenvaluemagnitude")=0.0)
        
        .def("count", &eigenvalue::count)
        
        .def("geteigenvaluerealpart", &eigenvalue::geteigenvaluerealpart)
        .def("geteigenvalueimaginarypart", &eigenvalue::geteigenvalueimaginarypart)
        .def("geteigenvectorrealpart", &eigenvalue::geteigenvectorrealpart)
        .def("geteigenvectorimaginarypart", &eigenvalue::geteigenvectorimaginarypart)

        .def("printeigenvalues", &eigenvalue::printeigenvalues)
        .def("printeigenfrequencies", &eigenvalue::printeigenfrequencies)
        
    ;
}

