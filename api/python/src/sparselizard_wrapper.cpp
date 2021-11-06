#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_densemat(py::module &);
void init_eigenvalue(py::module &);
void init_expression(py::module &);
void init_field(py::module &);
void init_formulation(py::module &);
void init_genalpha(py::module &);
void init_impliciteuler(py::module &);
void init_indexmat(py::module &);
void init_integration(py::module &);
void init_mat(py::module &);
void init_mesh(py::module &);
void init_parameter(py::module &);
void init_port(py::module &);
void init_shape(py::module &);
void init_sl(py::module &);
void init_slmpi(py::module &);
void init_spanningtree(py::module &);
void init_spline(py::module &);
void init_universe(py::module &);
void init_vec(py::module &);
void init_vectorfieldselect(py::module &);
void init_wallclock(py::module &);


PYBIND11_MODULE(spylizard, m)
{
    m.doc() = "This is the python binding for the sparselizard c++ library";
    m.def("printversion", &sl::printversion);
    m.def("getversion", &sl::getversion);
    m.def("getsubversion", &sl::getsubversion);
    m.def("getversionname", &sl::getversionname);

    init_densemat(m);
    init_eigenvalue(m);
    init_expression(m);
    init_field(m);
    init_formulation(m);
    init_genalpha(m);
    init_impliciteuler(m);
    init_indexmat(m);
    init_integration(m);
    init_mat(m);
    init_mesh(m);
    init_parameter(m);
    init_port(m);
    init_shape(m);
    init_sl(m);
    init_slmpi(m);
    init_spanningtree(m);
    init_spline(m);
    init_universe(m);
    init_vec(m);
    init_vectorfieldselect(m);
    init_wallclock(m);
}

