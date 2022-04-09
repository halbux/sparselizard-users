#include "sparselizard_wrapper.h"

namespace py = pybind11;

void init_sl(py::module &m)
{
    
    m.def("setmaxnumthreads", &sl::setmaxnumthreads, py::arg("mnt"));
    m.def("getmaxnumthreads", &sl::getmaxnumthreads);

    m.def("getpi", &sl::getpi);
    m.def("getrandom", &sl::getrandom);

    m.def("selectunion", &sl::selectunion, py::arg("physregs"));
    m.def("selectintersection", &sl::selectintersection, py::arg("physregs"));
    m.def("selectall", &sl::selectall);

    m.def("isdefined", &sl::isdefined, py::arg("physreg"));
    m.def("isempty", &sl::isempty, py::arg("physreg"));
    m.def("isinside", &sl::isinside, py::arg("physregtocheck"), py::arg("physreg"));
    m.def("istouching", &sl::istouching, py::arg("physregtocheck"), py::arg("physreg"));

    m.def("printvector", static_cast<void (*)(std::vector<double>)>(&sl::printvector), py::arg("input"));
    m.def("printvector", static_cast<void (*)(std::vector<int>)>(&sl::printvector), py::arg("input"));
    m.def("printvector", static_cast<void (*)(std::vector<bool>)>(&sl::printvector), py::arg("input"));

    m.def("writevector", &sl::writevector, py::arg("filename"), py::arg("towrite"), py::arg("delimiter")=',', py::arg("writesize")=false);
    m.def("loadvector", &sl::loadvector, py::arg("filename"), py::arg("delimiter")=',', py::arg("sizeincluded")=false);

    m.def("allpartition", &sl::allpartition, py::arg("meshfile"));

    m.def("norm", &sl::norm);

    m.def("normal", static_cast<expression (*)()>(&sl::normal));
    m.def("normal", static_cast<expression (*)(int)>(&sl::normal), py::arg("pointoutofphysreg"));
    
    m.def("tangent", &sl::tangent);

    m.def("scatterwrite", &sl::scatterwrite, py::arg("filename"), py::arg("xcoords"), py::arg("ycoords"), py::arg("zcoords"), py::arg("compxevals"), py::arg("compyevals")=std::vector<double>{}, py::arg("compzevals")=std::vector<double>{});

    m.def("setaxisymmetry", &sl::setaxisymmetry);

    m.def("setfundamentalfrequency", &sl::setfundamentalfrequency, py::arg("f"));
    
    m.def("settime", &sl::settime, py::arg("t"));
    m.def("gettime", &sl::gettime);

    m.def("meshsize", &sl::meshsize, py::arg("integrationorder"));
    m.def("fieldorder", &sl::fieldorder, py::arg("input"), py::arg("alpha")=-1.0, py::arg("absthres")=0.0);
    
    m.def("getharmonic", &sl::getharmonic, py::arg("harmnum"), py::arg("input"), py::arg("numfftharms")=-1);
    m.def("makeharmonic", &sl::makeharmonic, py::arg("harms"), py::arg("exprs"));
    m.def("moveharmonic", &sl::moveharmonic, py::arg("origharms"), py::arg("destharms"), py::arg("input"), py::arg("numfftharms")=-1);

    m.def("gettotalforce", static_cast<std::vector<double> (*)(int, expression, expression, int)>(&sl::gettotalforce), py::arg("physreg"), py::arg("EorH"), py::arg("epsilonormu"), py::arg("extraintegrationorder")=0);
    m.def("gettotalforce", static_cast<std::vector<double> (*)(int, expression, expression, expression, int)>(&sl::gettotalforce), py::arg("physreg"), py::arg("meshdeform"), py::arg("EorH"), py::arg("epsilonormu"), py::arg("extraintegrationorder")=0);

    m.def("printtotalforce", static_cast<std::vector<double> (*)(int, expression, expression, int)>(&sl::printtotalforce), py::arg("physreg"), py::arg("EorH"), py::arg("epsilonormu"), py::arg("extraintegrationorder")=0);
    m.def("printtotalforce", static_cast<std::vector<double> (*)(int, expression, expression, expression, int)>(&sl::printtotalforce), py::arg("physreg"), py::arg("meshdeform"), py::arg("EorH"), py::arg("epsilonormu"), py::arg("extraintegrationorder")=0);

    m.def("setphysicalregionshift", &sl::setphysicalregionshift, py::arg("shiftamount"));

    m.def("writeshapefunctions", &sl::writeshapefunctions, py::arg("filename"), py::arg("sftypename"), py::arg("elementtypenumber"), py::arg("maxorder"), py::arg("allorientations")=false);

    m.def("t", &sl::t);

    m.def("grouptimesteps", static_cast<void (*)(std::string, std::vector<std::string>, std::vector<double>)>(&sl::grouptimesteps), py::arg("filename"), py::arg("filestogroup"), py::arg("timevals"));
    m.def("grouptimesteps", static_cast<void (*)(std::string, std::string, int, std::vector<double>)>(&sl::grouptimesteps), py::arg("filename"), py::arg("fileprefix"), py::arg("firstint"), py::arg("timevals"));

    m.def("loadshape", &sl::loadshape, py::arg("meshfile"));

    m.def("settimederivative", static_cast<void (*)(vec)>(&sl::settimederivative), py::arg("dtx"));
    m.def("settimederivative", static_cast<void (*)(vec, vec)>(&sl::settimederivative), py::arg("dtx"), py::arg("dtdtx"));

    m.def("dx", &sl::dx, py::arg("input"));
    m.def("dy", &sl::dy, py::arg("input"));
    m.def("dz", &sl::dz, py::arg("input"));

    m.def("dt", static_cast<expression (*)(expression)>(&sl::dt), py::arg("input"));
    m.def("dtdt", static_cast<expression (*)(expression)>(&sl::dtdt), py::arg("input"));
    m.def("dtdtdt", &sl::dtdtdt, py::arg("input"));
    m.def("dtdtdtdt", &sl::dtdtdtdt, py::arg("input"));
    
    m.def("dt", static_cast<expression (*)(expression, double, double)>(&sl::dt), py::arg("input"), py::arg("initdt"), py::arg("initdtdt"));
    m.def("dtdt", static_cast<expression (*)(expression, double, double)>(&sl::dtdt), py::arg("input"), py::arg("initdt"), py::arg("initdtdt"));

    m.def("sin", &sl::sin, py::arg("input"));
    m.def("cos", &sl::cos, py::arg("input"));
    m.def("tan", &sl::tan, py::arg("input"));
    m.def("asin", &sl::asin, py::arg("input"));
    m.def("acos", &sl::acos, py::arg("input"));
    m.def("atan", &sl::atan, py::arg("input"));
    m.def("abs", &sl::abs, py::arg("input"));
    m.def("sqrt", &sl::sqrt, py::arg("input"));
    m.def("log10", &sl::log10, py::arg("input"));
    m.def("exp", &sl::exp, py::arg("input"));
    m.def("pow", &sl::pow, py::arg("base"), py::arg("exponent"));
    m.def("mod", &sl::mod, py::arg("input"), py::arg("modval"));

    m.def("ifpositive", &sl::ifpositive, py::arg("condexpr"), py::arg("trueexpr"), py::arg("falseexpr"));
    m.def("andpositive", &sl::andpositive, py::arg("exprs"));
    m.def("orpositive", &sl::orpositive, py::arg("exprs"));

    m.def("max", static_cast<expression (*)(expression, expression)>(&sl::max), py::arg("a"), py::arg("b"));
    m.def("max", static_cast<expression (*)(field, field)>(&sl::max), py::arg("a"), py::arg("b"));
    m.def("max", static_cast<expression (*)(parameter, parameter)>(&sl::max), py::arg("a"), py::arg("b"));
    
    m.def("min", static_cast<expression (*)(expression, expression)>(&sl::min), py::arg("a"), py::arg("b"));
    m.def("min", static_cast<expression (*)(field, field)>(&sl::min), py::arg("a"), py::arg("b"));
    m.def("min", static_cast<expression (*)(parameter, parameter)>(&sl::min), py::arg("a"), py::arg("b"));

    m.def("on", static_cast<expression (*)(int, expression, bool)>(&sl::on), py::arg("physreg"), py::arg("expression"), py::arg("errorifnotfound")=true);
    m.def("on", static_cast<expression (*)(int, expression, expression, bool)>(&sl::on), py::arg("physreg"), py::arg("coordshift"), py::arg("expression"), py::arg("errorifnotfound")=true);

    m.def("comp", &sl::comp, py::arg("selectedcomp"), py::arg("input"));
    m.def("compx", &sl::compx, py::arg("input"));
    m.def("compy", &sl::compy, py::arg("input"));
    m.def("compz", &sl::compz, py::arg("input"));

    m.def("entry", &sl::entry, py::arg("row"), py::arg("col"), py::arg("input"));
    m.def("eye", &sl::eye, py::arg("size"));

    m.def("transpose", &sl::transpose, py::arg("input"));
    m.def("inverse", &sl::inverse, py::arg("input"));
    m.def("determinant", &sl::determinant, py::arg("input"));

    m.def("grad", &sl::grad, py::arg("input"));
    m.def("div", &sl::div, py::arg("input"));
    m.def("curl", &sl::curl, py::arg("input"));

    m.def("crossproduct", &sl::crossproduct, py::arg("a"), py::arg("b"));
    m.def("doubledotproduct", &sl::doubledotproduct, py::arg("a"), py::arg("b"));
    m.def("trace", &sl::trace, py::arg("a"));

    m.def("integral", static_cast<integration (*)(int, expression, int, int)>(&sl::integral), py::arg("physreg"), py::arg("tointegrate"), py::arg("integrationorderdelta")=0, py::arg("blocknumber")=0);
    m.def("integral", static_cast<integration (*)(int, expression, expression, int, int)>(&sl::integral), py::arg("physreg"), py::arg("meshdeform"), py::arg("tointegrate"), py::arg("integrationorderdelta")=0, py::arg("blocknumber")=0);
    m.def("integral", static_cast<integration (*)(int, int, expression, int, int)>(&sl::integral), py::arg("physreg"), py::arg("numcoefharms"), py::arg("tointegrate"), py::arg("integrationorderdelta")=0, py::arg("blocknumber")=0);
    m.def("integral", static_cast<integration (*)(int, int, expression, expression, int, int)>(&sl::integral), py::arg("physreg"), py::arg("numcoefharms"), py::arg("meshdeform"), py::arg("tointegrate"), py::arg("integrationorderdelta")=0, py::arg("blocknumber")=0);

    m.def("dof", static_cast<expression (*)(expression)>(&sl::dof), py::arg("input"));
    m.def("dof", static_cast<expression (*)(expression, int)>(&sl::dof), py::arg("input"), py::arg("physreg"));

    m.def("tf", static_cast<expression (*)(expression)>(&sl::tf), py::arg("input"));
    m.def("tf", static_cast<expression (*)(expression, int)>(&sl::tf), py::arg("input"), py::arg("physreg"));

    m.def("adapt", &sl::adapt, py::arg("verbosity")=0);
    m.def("alladapt", &sl::alladapt, py::arg("verbosity")=0);
    
    m.def("zienkiewiczzhu", &sl::zienkiewiczzhu, py::arg("input"));

    m.def("array1x1", &sl::array1x1);
    m.def("array1x2", &sl::array1x2);
    m.def("array1x3", &sl::array1x3);
    m.def("array2x1", &sl::array2x1);
    m.def("array2x2", &sl::array2x2);
    m.def("array2x3", &sl::array2x3);
    m.def("array3x1", &sl::array3x1);
    m.def("array3x2", &sl::array3x2);
    m.def("array3x3", &sl::array3x3);

    m.def("solve", static_cast<vec (*)(mat, vec, std::string, bool)>(&sl::solve), py::arg("A"), py::arg("b"), py::arg("soltype")="lu", py::arg("diagscaling")=false);
    m.def("solve", static_cast<std::vector<vec> (*)(mat, std::vector<vec>, std::string)>(&sl::solve), py::arg("A"), py::arg("b"), py::arg("soltype")="lu");
    m.def("solve", static_cast<void (*)(mat, vec, vec, double&, int&, std::string, std::string, int, bool)>(&sl::solve), py::arg("A"), py::arg("b"), py::arg("sol"), py::arg("relrestol"), py::arg("maxnumit"), py::arg("soltype")="bicgstab", py::arg("precondtype")="sor", py::arg("verbosity")=1, py::arg("diagscaling")=false);

    m.def("linspace", &sl::linspace, py::arg("a"), py::arg("b"), py::arg("num"));
    m.def("logspace", &sl::logspace, py::arg("a"), py::arg("b"), py::arg("num"), py::arg("basis")=10.0);

    m.def("dbtoneper", &sl::dbtoneper, py::arg("toconvert"));

    m.def("setdata", &sl::setdata, py::arg("invec"));

    m.def("strain", &sl::strain, py::arg("input"));
    m.def("greenlagrangestrain", &sl::greenlagrangestrain, py::arg("input"));
    m.def("vonmises", &sl::vonmises, py::arg("stress"));

    m.def("continuitycondition", static_cast<std::vector<integration> (*)(int, int, field, field, int, bool)>(&sl::continuitycondition), py::arg("gamma1"), py::arg("gamma2"), py::arg("u1"), py::arg("u2"), py::arg("lagmultorder"), py::arg("errorifnotfound")=true);
    m.def("continuitycondition", static_cast<std::vector<integration> (*)(int, int, field, field, std::vector<double>, double, double, double, int)>(&sl::continuitycondition), py::arg("gamma1"), py::arg("gamma2"), py::arg("u1"), py::arg("u2"), py::arg("rotcent"), py::arg("rotangz"), py::arg("angzmod"), py::arg("factor"), py::arg("lagmultorder"));

    m.def("periodicitycondition", &sl::periodicitycondition, py::arg("gamma1"), py::arg("gamma2"), py::arg("u"), py::arg("dat1"), py::arg("dat2"), py::arg("factor"), py::arg("lagmultorder"));

    m.def("predefinedelasticity", static_cast<expression (*)(expression, expression, expression, expression, std::string)>(&sl::predefinedelasticity), py::arg("dofu"), py::arg("tfu"), py::arg("Eyoung"), py::arg("nupoisson"), py::arg("myoption")="");
    m.def("predefinedelasticity", static_cast<expression (*)(expression, expression, expression, std::string)>(&sl::predefinedelasticity), py::arg("dofu"), py::arg("tfu"), py::arg("elasticitymatrix"), py::arg("myoption")="");
    m.def("predefinedelasticity", static_cast<expression (*)(expression, expression, field, expression, expression, expression, std::string)>(&sl::predefinedelasticity), py::arg("dofu"), py::arg("tfu"), py::arg("u"), py::arg("Eyoung"), py::arg("nupoisson"), py::arg("prestress"), py::arg("myoption")="");
    m.def("predefinedelasticity", static_cast<expression (*)(expression, expression, field, expression, expression, std::string)>(&sl::predefinedelasticity), py::arg("dofu"), py::arg("tfu"), py::arg("u"), py::arg("elasticitymatrix"), py::arg("prestress"), py::arg("myoption")="");

    m.def("predefinedelectrostaticforce", &sl::predefinedelectrostaticforce, py::arg("input"), py::arg("E"), py::arg("epsilon"));
    m.def("predefinedmagnetostaticforce", &sl::predefinedmagnetostaticforce, py::arg("input"), py::arg("H"), py::arg("mu"));

    m.def("predefinedacousticwave", &sl::predefinedacousticwave, py::arg("dofp"), py::arg("tfp"), py::arg("soundspeed"), py::arg("neperattenuation"));
    m.def("predefinedacousticradiation", &sl::predefinedacousticradiation, py::arg("dofp"), py::arg("tfp"), py::arg("soundspeed"), py::arg("neperattenuation"));
    m.def("predefinedacousticstructureinteraction", &sl::predefinedacousticstructureinteraction, py::arg("dofp"), py::arg("tfp"), py::arg("dofu"), py::arg("tfu"), py::arg("soundspeed"), py::arg("fluiddensity"), py::arg("normal"), py::arg("neperattenuation"), py::arg("scaling")=1.0);

    m.def("predefinedstokes", &sl::predefinedstokes, py::arg("dofv"), py::arg("tfv"), py::arg("dofp"), py::arg("tfp"), py::arg("mu"), py::arg("rho"), py::arg("dtrho"), py::arg("gradrho"), py::arg("includetimederivs")=false, py::arg("isdensityconstant")=true, py::arg("isviscosityconstant")=true);
    m.def("predefinednavierstokes", &sl::predefinednavierstokes, py::arg("dofv"), py::arg("tfv"), py::arg("v"), py::arg("dofp"), py::arg("tfp"), py::arg("mu"), py::arg("rho"), py::arg("dtrho"), py::arg("gradrho"), py::arg("includetimederivs")=false, py::arg("isdensityconstant")=true, py::arg("isviscosityconstant")=true);

    m.def("predefinedadvectiondiffusion", &sl::predefinedadvectiondiffusion, py::arg("doff"), py::arg("tff"), py::arg("v"), py::arg("alpha"), py::arg("beta"), py::arg("gamma"), py::arg("isdivvzero")=true);
    m.def("predefineddiffusion", &sl::predefineddiffusion, py::arg("doff"), py::arg("tff"), py::arg("alpha"), py::arg("beta"));

    m.def("predefinedstabilization", &sl::predefinedstabilization, py::arg("stabtype"), py::arg("delta"), py::arg("f"), py::arg("v"), py::arg("diffusivity"), py::arg("residual"));

}

