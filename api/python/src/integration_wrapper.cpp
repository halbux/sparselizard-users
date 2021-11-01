#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_integration(py::module &m)
{
    py::class_<integration>(m, "integration")
        
    ;
}

