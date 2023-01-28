#include "sparselizard_wrapper.h"

PYBIND11_MAKE_OPAQUE(std::vector<bool>);
PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::vector<double>);

namespace py = pybind11;

void init_slmpi(py::module &m)
{
    py::bind_vector<std::vector<bool> >(m, "VecBool");
    py::bind_vector<std::vector<int> >(m, "VecInt");
    py::bind_vector<std::vector<double> >(m, "VecDouble");

    m.def("isavailable", &slmpi::isavailable);
    
    m.def("initialize", &slmpi::initialize);
    m.def("finalize", &slmpi::finalize);
    
    m.def("getrank", &slmpi::getrank);
    m.def("count", &slmpi::count);
    
    m.def("barrier", &slmpi::barrier);

    m.def("send", static_cast<void (*)(int, int, std::vector<int>&)>(&slmpi::send), py::arg("destination"), py::arg("tag"), py::arg("data"));
    m.def("send", static_cast<void (*)(int, int, std::vector<double>&)>(&slmpi::send), py::arg("destination"), py::arg("tag"), py::arg("data"));
    
    m.def("receive", static_cast<void (*)(int, int, std::vector<int>&)>(&slmpi::receive), py::arg("source"), py::arg("tag"), py::arg("data"));
    m.def("receive", static_cast<void (*)(int, int, std::vector<double>&)>(&slmpi::receive), py::arg("source"), py::arg("tag"), py::arg("data"));
    
    m.def("sum", static_cast<void (*)(std::vector<int>&)>(&slmpi::sum), py::arg("data"));
    m.def("sum", static_cast<void (*)(std::vector<double>&)>(&slmpi::sum), py::arg("data"));
    
    m.def("max", static_cast<void (*)(std::vector<int>&)>(&slmpi::max), py::arg("data"));
    m.def("max", static_cast<void (*)(std::vector<double>&)>(&slmpi::max), py::arg("data"));
    
    m.def("min", static_cast<void (*)(std::vector<int>&)>(&slmpi::min), py::arg("data"));
    m.def("min", static_cast<void (*)(std::vector<double>&)>(&slmpi::min), py::arg("data"));

    m.def("broadcast", static_cast<void (*)(int, std::vector<int>&)>(&slmpi::broadcast), py::arg("broadcaster"), py::arg("data"));
    m.def("broadcast", static_cast<void (*)(int, std::vector<double>&)>(&slmpi::broadcast), py::arg("broadcaster"), py::arg("data"));
    
    m.def("gather", static_cast<void (*)(int, std::vector<int>&, std::vector<int>&)>(&slmpi::gather), py::arg("gatherer"), py::arg("fragment"), py::arg("gathered"));
    m.def("gather", static_cast<void (*)(int, std::vector<double>&, std::vector<double>&)>(&slmpi::gather), py::arg("gatherer"), py::arg("fragment"), py::arg("gathered"));
    m.def("gather", static_cast<void (*)(int, std::vector<int>&, std::vector<int>&, std::vector<int>&)>(&slmpi::gather), py::arg("gatherer"), py::arg("fragment"), py::arg("gathered"), py::arg("fragsizes"));
    m.def("gather", static_cast<void (*)(int, std::vector<double>&, std::vector<double>&, std::vector<int>&)>(&slmpi::gather), py::arg("gatherer"), py::arg("fragment"), py::arg("gathered"), py::arg("fragsizes"));
    
    m.def("allgather", static_cast<void (*)(std::vector<int>&, std::vector<int>&)>(&slmpi::allgather), py::arg("fragment"), py::arg("gathered"));
    m.def("allgather", static_cast<void (*)(std::vector<double>&, std::vector<double>&)>(&slmpi::allgather), py::arg("fragment"), py::arg("gathered"));
    m.def("allgather", static_cast<void (*)(std::vector<int>&, std::vector<int>&, std::vector<int>&)>(&slmpi::allgather), py::arg("fragment"), py::arg("gathered"), py::arg("fragsizes"));
    m.def("allgather", static_cast<void (*)(std::vector<double>&, std::vector<double>&, std::vector<int>&)>(&slmpi::allgather), py::arg("fragment"), py::arg("gathered"), py::arg("fragsizes"));
    
    m.def("scatter", static_cast<void (*)(int, std::vector<int>&, std::vector<int>&)>(&slmpi::scatter), py::arg("scatterer"), py::arg("toscatter"), py::arg("fragment"));
    m.def("scatter", static_cast<void (*)(int, std::vector<double>&, std::vector<double>&)>(&slmpi::scatter), py::arg("scatterer"), py::arg("toscatter"), py::arg("fragment"));
    m.def("scatter", static_cast<void (*)(int, std::vector<int>&, std::vector<int>&, std::vector<int>&)>(&slmpi::scatter), py::arg("scatterer"), py::arg("toscatter"), py::arg("fragment"), py::arg("fragsizes"));
    m.def("scatter", static_cast<void (*)(int, std::vector<double>&, std::vector<double>&, std::vector<int>&)>(&slmpi::scatter), py::arg("scatterer"), py::arg("toscatter"), py::arg("fragment"), py::arg("fragsizes"));
    
    m.def("exchange", static_cast<void (*)(std::vector<int>, std::vector<int>&, std::vector<int>&)>(&slmpi::exchange), py::arg("targetranks"), py::arg("sendvalues"), py::arg("receivevalues"));
    m.def("exchange", static_cast<void (*)(std::vector<int>, std::vector<double>&, std::vector<double>&)>(&slmpi::exchange), py::arg("targetranks"), py::arg("sendvalues"), py::arg("receivevalues"));
    m.def("exchange", static_cast<void (*)(std::vector<int>, std::vector<std::vector<int>>&, std::vector<std::vector<int>>&)>(&slmpi::exchange), py::arg("targetranks"), py::arg("sends"), py::arg("receives"));
    m.def("exchange", static_cast<void (*)(std::vector<int>, std::vector<std::vector<double>>&, std::vector<std::vector<double>>&)>(&slmpi::exchange), py::arg("targetranks"), py::arg("sends"), py::arg("receives"));
    
}

