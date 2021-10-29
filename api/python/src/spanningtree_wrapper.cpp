#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_spanningtree(py::module &m)
{
    py::class_<spanningtree>(m, "spanningtree")
    
        .def(py::init<std::vector<int>>(), py::arg("physregs"))

        .def("countedgesintree", &spanningtree::countedgesintree)
        
        .def("write", &spanningtree::write, py::arg("filename"))
        
    ;
}

