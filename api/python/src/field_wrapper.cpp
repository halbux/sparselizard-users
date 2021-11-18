#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_field(py::module &m)
{
    py::class_<field>(m, "field")
    
        .def(py::init<>())
        .def(py::init<std::string>(), py::arg("fieldtypename"))
        .def(py::init<std::string, const std::vector<int>>(), py::arg("fieldtypename"), py::arg("harmonicnumbers"))
        .def(py::init<std::string, spanningtree>(), py::arg("fieldtypename"), py::arg("spantree"))
        .def(py::init<std::string, const std::vector<int>, spanningtree>(), py::arg("fieldtypename"), py::arg("harmonicnumbers"), py::arg("spantree"))

        .def("countcomponents", &field::countcomponents)
        .def("getharmonics", &field::getharmonics)
        .def("printharmonics", &field::printharmonics)
        .def("setname", &field::setname, py::arg("name"))
        .def("print", &field::print)

        .def("setorder", static_cast<void (field::*)(int, int)>(&field::setorder), py::arg("physreg"), py::arg("interpolorder"))
        .def("setorder", static_cast<void (field::*)(expression, int, int)>(&field::setorder), py::arg("criterion"), py::arg("loworder"), py::arg("highorder"))
        .def("setorder", static_cast<void (field::*)(double, int, int, double)>(&field::setorder), py::arg("targeterror"), py::arg("loworder"), py::arg("highorder"), py::arg("absthres"))

        .def("setport", &field::setport, py::arg("physreg"), py::arg("primal"), py::arg("dual"))

        .def("setvalue", static_cast<void (field::*)(int, expression, int)>(&field::setvalue), py::arg("physreg"), py::arg("input"), py::arg("extraintegrationdegree")=0)
        .def("setvalue", static_cast<void (field::*)(int, expression, expression, int)>(&field::setvalue), py::arg("physreg"), py::arg("meshdeform"), py::arg("input"), py::arg("extraintegrationdegree")=0)
        .def("setvalue", static_cast<void (field::*)(int, int, expression, int)>(&field::setvalue), py::arg("physreg"), py::arg("numfftharms"), py::arg("input"), py::arg("extraintegrationdegree")=0)
        .def("setvalue", static_cast<void (field::*)(int, int, expression, expression, int)>(&field::setvalue), py::arg("physreg"), py::arg("numfftharms"), py::arg("meshdeform"), py::arg("input"), py::arg("extraintegrationdegree")=0)
        .def("setvalue", static_cast<void (field::*)(int)>(&field::setvalue), py::arg("physreg"))

        .def("setnodalvalues", &field::setnodalvalues, py::arg("nodenumbers"), py::arg("values"))
        .def("getnodalvalues", &field::getnodalvalues, py::arg("nodenumbers"))
        
        .def("setconstraint", static_cast<void (field::*)(int, expression, int)>(&field::setconstraint), py::arg("physreg"), py::arg("input"), py::arg("extraintegrationdegree")=0)
        .def("setconstraint", static_cast<void (field::*)(int, expression, expression, int)>(&field::setconstraint), py::arg("physreg"), py::arg("meshdeform"), py::arg("input"), py::arg("extraintegrationdegree")=0)
        .def("setconstraint", static_cast<void (field::*)(int, std::vector<expression>, int)>(&field::setconstraint), py::arg("physreg"), py::arg("input"), py::arg("extraintegrationdegree")=0)
        .def("setconstraint", static_cast<void (field::*)(int, expression, std::vector<expression>, int)>(&field::setconstraint), py::arg("physreg"), py::arg("meshdeform"), py::arg("input"), py::arg("extraintegrationdegree")=0)
        .def("setconstraint", static_cast<void (field::*)(int, int, expression, int)>(&field::setconstraint), py::arg("physreg"), py::arg("numfftharms"), py::arg("input"), py::arg("extraintegrationdegree")=0)
        .def("setconstraint", static_cast<void (field::*)(int, int, expression, expression, int)>(&field::setconstraint), py::arg("physreg"), py::arg("numfftharms"), py::arg("meshdeform"), py::arg("input"), py::arg("extraintegrationdegree")=0)
        .def("setconstraint", static_cast<void (field::*)(int)>(&field::setconstraint), py::arg("physreg"))

        .def("setconditionalconstraint", &field::setconditionalconstraint, py::arg("physreg"), py::arg("condexpr"), py::arg("valexpr"))
        .def("setgauge", &field::setgauge, py::arg("physreg"))

        .def("setdata", static_cast<void (field::*)(int, vec, std::string)>(&field::setdata), py::arg("physreg"), py::arg("myvec"), py::arg("op")="set")
        .def("setdata", static_cast<void (field::*)(int, vectorfieldselect, std::string)>(&field::setdata), py::arg("physreg"), py::arg("myvec"), py::arg("op")="set")
        
        .def("setcohomologysources", &field::setcohomologysources, py::arg("cutphysregs"), py::arg("cutvalues"))

        .def("automaticupdate", &field::automaticupdate, py::arg("updateit"))
        .def("noautomaticupdate", &field::noautomaticupdate)

        .def("setupdateaccuracy", &field::setupdateaccuracy, py::arg("extraintegrationorder"))

        .def("comp", &field::comp, py::arg("component"))
        .def("compx", &field::compx)
        .def("compy", &field::compy)
        .def("compz", &field::compz)

        .def("harmonic", static_cast<field (field::*)(int)>(&field::harmonic), py::arg("harmonicnumber"))
        .def("harmonic", static_cast<field (field::*)(const std::vector<int>)>(&field::harmonic), py::arg("harmonicnumbers"))

        .def("sin", &field::sin, py::arg("freqindex"))
        .def("cos", &field::cos, py::arg("freqindex"))

        .def("atbarycenter", &field::atbarycenter, py::arg("physreg"), py::arg("onefield"))

        .def("max", static_cast<std::vector<double> (field::*)(int, int, std::vector<double>)>(&field::max), py::arg("physreg"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})
        .def("max", static_cast<std::vector<double> (field::*)(int, expression, int, std::vector<double>)>(&field::max), py::arg("physreg"), py::arg("meshdeform"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})
        .def("min", static_cast<std::vector<double> (field::*)(int, int, std::vector<double>)>(&field::min), py::arg("physreg"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})
        .def("min", static_cast<std::vector<double> (field::*)(int, expression, int, std::vector<double>)>(&field::min), py::arg("physreg"), py::arg("meshdeform"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})

        .def("interpolate", static_cast<void (field::*)(int, std::vector<double>&, std::vector<double>&, std::vector<bool>&)>(&field::interpolate), py::arg("physreg"), py::arg("xyzcoord"), py::arg("interpolated"), py::arg("isfound"))
        .def("interpolate", static_cast<void (field::*)(int, expression, std::vector<double>&, std::vector<double>&, std::vector<bool>&)>(&field::interpolate), py::arg("physreg"), py::arg("meshdeform"), py::arg("xyzcoord"), py::arg("interpolated"), py::arg("isfound"))
        .def("interpolate", static_cast<std::vector<double> (field::*)(int, const std::vector<double>)>(&field::interpolate), py::arg("physreg"), py::arg("xyzcoord"))
        .def("interpolate", static_cast<std::vector<double> (field::*)(int, expression, const std::vector<double>)>(&field::interpolate), py::arg("physreg"), py::arg("meshdeform"), py::arg("xyzcoord"))

        .def("integrate", static_cast<double (field::*)(int, int)>(&field::integrate), py::arg("physreg"), py::arg("integrationorder"))
        .def("integrate", static_cast<double (field::*)(int, expression, int)>(&field::integrate), py::arg("physreg"), py::arg("meshdeform"), py::arg("integrationorder"))

        .def("write", static_cast<void (field::*)(int, int, std::string, int)>(&field::write), py::arg("physreg"), py::arg("numfftharms"), py::arg("filename"), py::arg("lagrangeorder"))
        .def("write", static_cast<void (field::*)(int, int, expression, std::string, int)>(&field::write), py::arg("physreg"), py::arg("numfftharms"), py::arg("meshdeform"), py::arg("filename"), py::arg("lagrangeorder"))
        .def("write", static_cast<void (field::*)(int, std::string, int, int)>(&field::write), py::arg("physreg"), py::arg("filename"), py::arg("lagrangeorder"), py::arg("numtimesteps")=-1)
        .def("write", static_cast<void (field::*)(int, expression, std::string, int, int)>(&field::write), py::arg("physreg"), py::arg("meshdeform"), py::arg("filename"), py::arg("lagrangeorder"), py::arg("numtimesteps")=-1)

        .def("writeraw", &field::writeraw, py::arg("physreg"), py::arg("filename"), py::arg("isbinary")=false, py::arg("extradata")=std::vector<double>{})
        .def("loadraw", &field::loadraw, py::arg("filename"), py::arg("isbinary")=false)
        
        
        .def("__pos__", static_cast<expression (field::*)()>(&field::operator+))
        .def("__neg__", static_cast<expression (field::*)()>(&field::operator-))

        // operator (field, field)
        .def("__add__", [](field &a, field &b) { return a+b;}, py::is_operator())
        .def("__sub__", [](field &a, field &b) { return a-b;}, py::is_operator())
        .def("__mul__", [](field &a, field &b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](field &a, field &b) { return a/b;}, py::is_operator())

        // operator (field, double)
        .def("__add__", [](field &a, double b) { return a+b;}, py::is_operator())
        .def("__sub__", [](field &a, double b) { return a-b;}, py::is_operator())
        .def("__mul__", [](field &a, double b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](field &a, double b) { return a/b;}, py::is_operator())

        // operator (field, parameter)
        .def("__add__", [](field &a, parameter &b) { return a+b;}, py::is_operator())
        .def("__sub__", [](field &a, parameter &b) { return a-b;}, py::is_operator())
        .def("__mul__", [](field &a, parameter &b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](field &a, parameter &b) { return a/b;}, py::is_operator())

        // operator (double, field)
        .def("__radd__", [](field &a, double b) { return b+a;}, py::is_operator())
        .def("__rsub__", [](field &a, double b) { return b-a;}, py::is_operator())
        .def("__rmul__", [](field &a, double b) { return b*a;}, py::is_operator())
        .def("__rtruediv__", [](field &a, double b) { return b/a;}, py::is_operator())

        // operator (parameter, field)
        .def("__radd__", [](field &a, parameter &b) { return b+a;}, py::is_operator())
        .def("__rsub__", [](field &a, parameter &b) { return b-a;}, py::is_operator())
        .def("__rmul__", [](field &a, parameter &b) { return b*a;}, py::is_operator())
        .def("__rtruediv__", [](field &a, parameter &b) { return b/a;}, py::is_operator())
        
    ;
}

