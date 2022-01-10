#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_expression(py::module &m)
{
    py::class_<expression>(m, "expression")
    
        .def(py::init<>())
        .def(py::init<field>(), py::arg("input"))
        .def(py::init<double>(), py::arg("input"))
        .def(py::init<parameter>(), py::arg("input"))
        .def(py::init<port>(), py::arg("input"))
        .def(py::init<int, int, std::vector<expression>>(), py::arg("numrows"), py::arg("numcols"), py::arg("exprs"))
        .def(py::init<const std::vector<std::vector<expression>>>(), py::arg("input"))
        .def(py::init<expression, expression, expression>(), py::arg("condexpr"), py::arg("exprtrue"), py::arg("exprfalse"))
        .def(py::init<spline, expression>(), py::arg("spl"), py::arg("arg"))
        .def(py::init<std::vector<double>, std::vector<expression>, expression>(), py::arg("pos"), py::arg("exprs"), py::arg("tocompare"))

        .def("countrows", &expression::countrows)
        .def("countcolumns", &expression::countcolumns)

        .def("getrow", &expression::getrow, py::arg("rownum"))
        .def("getcolumn", &expression::getcolumn, py::arg("colnum"))

        .def("reorderrows", &expression::reorderrows, py::arg("neworder"))
        .def("reordercolumns", &expression::reordercolumns, py::arg("neworder"))

        .def("max", static_cast<std::vector<double> (expression::*)(int, int, std::vector<double>)>(&expression::max), py::arg("physreg"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})
        .def("max", static_cast<std::vector<double> (expression::*)(int, expression, int, std::vector<double>)>(&expression::max), py::arg("physreg"), py::arg("meshdeform"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})
        .def("min", static_cast<std::vector<double> (expression::*)(int, int, std::vector<double>)>(&expression::min), py::arg("physreg"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})
        .def("min", static_cast<std::vector<double> (expression::*)(int, expression, int, std::vector<double>)>(&expression::min), py::arg("physreg"), py::arg("meshdeform"), py::arg("refinement"), py::arg("xyzrange")=std::vector<double>{})

        .def("interpolate", [](expression &self, int physreg, py::list& xyzcoord, py::list& interpolated, py::list& isfound){
                std::cout<<"running interpolate wrapper"<<std::endl;
                const unsigned int numPoints = py::len(xyzcoord);
                std::vector<double> interpolatedWrapped(numPoints);
                std::vector<double> evalPointsWrapped(numPoints);
                std::vector<bool> isFoundWrapped(numPoints);
                evalPointsWrapped.clear();
                for (auto& el : xyzcoord)
                    evalPointsWrapped.push_back(el.cast<double>());
                self.interpolate(physreg, evalPointsWrapped, interpolatedWrapped, isFoundWrapped);
                if (evalPointsWrapped.size() != numPoints)
                    std::cout<<"return size mismatch, expected "<<numPoints<<" got "<<evalPointsWrapped.size()<<" items!"<<std::endl;
                for(size_t index = 0; index < numPoints; index++){
                    interpolated.append(interpolatedWrapped[index]);
                    isfound.append(isFoundWrapped[index] == true ? true : false);   // weird std::vector<bool> needs this trick
                }
            }, py::arg("physreg"), py::arg("xyzcoord"), py::arg("interpolated"), py::arg("isfound"))
        .def("interpolate", static_cast<void (expression::*)(int, expression, std::vector<double>&, std::vector<double>&, std::vector<bool>&)>(&expression::interpolate), py::arg("physreg"), py::arg("meshdeform"), py::arg("xyzcoord"), py::arg("interpolated"), py::arg("isfound"))
        .def("interpolate", static_cast<std::vector<double> (expression::*)(int, const std::vector<double>)>(&expression::interpolate), py::arg("physreg"), py::arg("xyzcoord"))
        .def("interpolate", static_cast<std::vector<double> (expression::*)(int, expression, const std::vector<double>)>(&expression::interpolate), py::arg("physreg"), py::arg("meshdeform"), py::arg("xyzcoord"))

        .def("integrate", static_cast<double (expression::*)(int, int)>(&expression::integrate), py::arg("physreg"), py::arg("integrationorder"))
        .def("integrate", static_cast<double (expression::*)(int, expression, int)>(&expression::integrate), py::arg("physreg"), py::arg("meshdeform"), py::arg("integrationorder"))

        .def("write", static_cast<void (expression::*)(int, int, std::string, int)>(&expression::write), py::arg("physreg"), py::arg("numfftharms"), py::arg("filename"), py::arg("lagrangeorder"))
        .def("write", static_cast<void (expression::*)(int, int, expression, std::string, int)>(&expression::write), py::arg("physreg"), py::arg("numfftharms"), py::arg("meshdeform"), py::arg("filename"), py::arg("lagrangeorder"))
        .def("write", static_cast<void (expression::*)(int, std::string, int, int)>(&expression::write), py::arg("physreg"), py::arg("filename"), py::arg("lagrangeorder"), py::arg("numtimesteps")=-1)
        .def("write", static_cast<void (expression::*)(int, expression, std::string, int, int)>(&expression::write), py::arg("physreg"), py::arg("meshdeform"), py::arg("filename"), py::arg("lagrangeorder"), py::arg("numtimesteps")=-1)

        .def("streamline", &expression::streamline, py::arg("physreg"), py::arg("filename"), py::arg("startcoords"), py::arg("stepsize"), py::arg("downstreamonly")=false)

        .def("reuseit", &expression::reuseit, py::arg("istobereused")=true)

        .def("isscalar", &expression::isscalar)
        .def("iszero", &expression::iszero)

        .def("atbarycenter", &expression::atbarycenter, py::arg("physreg"), py::arg("onefield"))

        .def("print", &expression::print)

        .def("rotate", &expression::rotate, py::arg("ax"), py::arg("ay"), py::arg("az"), py::arg("leftop")="default", py::arg("rightop")="default")

        .def("at", &expression::at, py::arg("row"), py::arg("col"))

        .def("evaluate", static_cast<double (expression::*)()>(&expression::evaluate))

        .def("resize", &expression::resize, py::arg("numrows"), py::arg("numcols"))


        .def("__pos__", static_cast<expression (expression::*)()>(&expression::operator+))
        .def("__neg__", static_cast<expression (expression::*)()>(&expression::operator-))

        // operator (expression, expression)
        .def("__add__", [](expression &a, expression &b) { return a+b;}, py::is_operator())
        .def("__sub__", [](expression &a, expression &b) { return a-b;}, py::is_operator())
        .def("__mul__", [](expression &a, expression &b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](expression &a, expression &b) { return a/b;}, py::is_operator())

        // operator (expression, field)
        .def("__add__", [](expression &a, field &b) { return a+b;}, py::is_operator())
        .def("__sub__", [](expression &a, field &b) { return a-b;}, py::is_operator())
        .def("__mul__", [](expression &a, field &b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](expression &a, field &b) { return a/b;}, py::is_operator())

        // operator (expression, double)
        .def("__add__", [](expression &a, double b) { return a+b;}, py::is_operator())
        .def("__sub__", [](expression &a, double b) { return a-b;}, py::is_operator())
        .def("__mul__", [](expression &a, double b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](expression &a, double b) { return a/b;}, py::is_operator())

        // operator (expression, parameter)
        .def("__add__", [](expression &a, parameter &b) { return a+b;}, py::is_operator())
        .def("__sub__", [](expression &a, parameter &b) { return a-b;}, py::is_operator())
        .def("__mul__", [](expression &a, parameter &b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](expression &a, parameter &b) { return a/b;}, py::is_operator())

        // operator (expression, port)
        .def("__add__", [](expression &a, port &b) { return a+b;}, py::is_operator())
        .def("__sub__", [](expression &a, port &b) { return a-b;}, py::is_operator())
        .def("__mul__", [](expression &a, port &b) { return a*b;}, py::is_operator())
        .def("__truediv__", [](expression &a, port &b) { return a/b;}, py::is_operator())
        
        // operator (double, expression)
        .def("__radd__", [](expression &a, double b) { return b+a;}, py::is_operator())
        .def("__rsub__", [](expression &a, double b) { return b-a;}, py::is_operator())
        .def("__rmul__", [](expression &a, double b) { return b*a;}, py::is_operator())
        .def("__rtruediv__", [](expression &a, double b) { return b/a;}, py::is_operator())

        // operator (field, expression)
        .def("__radd__", [](expression &a, field &b) { return b+a;}, py::is_operator())
        .def("__rsub__", [](expression &a, field &b) { return b-a;}, py::is_operator())
        .def("__rmul__", [](expression &a, field &b) { return b*a;}, py::is_operator())
        .def("__rtruediv__", [](expression &a, field &b) { return b/a;}, py::is_operator())

        // operator (parameter, expression)
        .def("__radd__", [](expression &a, parameter &b) { return b+a;}, py::is_operator())
        .def("__rsub__", [](expression &a, parameter &b) { return b-a;}, py::is_operator())
        .def("__rmul__", [](expression &a, parameter &b) { return b*a;}, py::is_operator())
        .def("__rtruediv__", [](expression &a, parameter &b) { return b/a;}, py::is_operator())

        // operator (port, expression)
        .def("__radd__", [](expression &a, port &b) { return b+a;}, py::is_operator())
        .def("__rsub__", [](expression &a, port &b) { return b-a;}, py::is_operator())
        .def("__rmul__", [](expression &a, port &b) { return b*a;}, py::is_operator())
        .def("__rtruediv__", [](expression &a, port &b) { return b/a;}, py::is_operator())
        
    ;

    py::implicitly_convertible<double, expression>();
    py::implicitly_convertible<int, expression>();
    py::implicitly_convertible<parameter, expression>();
    py::implicitly_convertible<field, expression>();
    py::implicitly_convertible<port, expression>();
}

