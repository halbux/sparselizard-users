#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_wallclock(py::module &m)
{
    py::class_<wallclock>(m, "wallclock")
    
        .def(py::init<>())
        .def("tic", &wallclock::tic)
        .def("toc", &wallclock::toc)
        .def("print", &wallclock::print, py::arg("toprint")="")
        .def("pause", &wallclock::pause)
        .def("resume", &wallclock::resume)
        
    ;
}

