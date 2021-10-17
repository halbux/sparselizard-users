#include "sparselizard_wrapper.h"

namespace py = pybind11;

using namespace pybind11::literals;
namespace py = pybind11;

void init_vec(py::module &);
void init_mat(py::module &);
void init_mesh(py::module &);
void init_expression(py::module &);
void init_parameter(py::module &);
void init_parameterselectedregion(py::module &);
void init_field(py::module &);
void init_port(py::module &);
void init_integration(py::module &);
void init_formulation(py::module &);
void init_genalpha(py::module &);
void init_impliciteuler(py::module &);
void init_universe(py::module &);
void init_wallclock(py::module &);
void init_sl(py::module &);


PYBIND11_MODULE(pysparselizard, m) {

    m.doc() = "This is a Python binding of C++ Sparselizard Library";
    m.def("printversion", &sl::printversion);
    m.def("getversion", &sl::getversion);
    m.def("getsubversion", &sl::getsubversion);
    m.def("getversionname", &sl::getversionname);

    init_vec(m);
    init_mat(m);
    init_mesh(m);
    init_expression(m);
    init_parameter(m);
    init_parameterselectedregion(m);
    init_field(m);
    init_port(m);
    init_integration(m);
    init_formulation(m);
    init_universe(m);
    init_genalpha(m);
    init_impliciteuler(m);
    init_sl(m);
    init_wallclock(m);

    py::implicitly_convertible<double, expression>();
    py::implicitly_convertible<int, expression>();
    py::implicitly_convertible<parameter, expression>();
    py::implicitly_convertible<field, expression>();
    py::implicitly_convertible<port, expression>();
}
