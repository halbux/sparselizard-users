#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_parameterselectedregion(py::module &m)
{
    //py::class_<parameter, std::shared_ptr<parameter> >(m, "parameter")
    py::class_<parameterselectedregion>(m, "parameterselectedregion")
        .def("__set__", static_cast<void (parameterselectedregion::*)(expression)>(&parameterselectedregion::operator=))
    ;
}
