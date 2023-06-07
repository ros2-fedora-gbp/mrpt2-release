#include <iterator>
#include <memory>
#include <mrpt/math/CMatrixFixed.h>
#include <mrpt/math/CQuaternion.h>
#include <mrpt/math/TPoint2D.h>
#include <mrpt/math/TPoint3D.h>
#include <mrpt/math/TPose2D.h>
#include <mrpt/math/TPose3D.h>
#include <mrpt/math/TTwist3D.h>
#include <optional>
#include <sstream> // __str__
#include <string>

#include <functional>
#include <pybind11/pybind11.h>
#include <string>
#include <stl_binders.hpp>


#ifndef BINDER_PYBIND11_TYPE_CASTER
	#define BINDER_PYBIND11_TYPE_CASTER
	PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>)
	PYBIND11_DECLARE_HOLDER_TYPE(T, T*)
	PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>)
#endif

void bind_mrpt_math_TTwist3D(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // mrpt::math::TTwist3D file:mrpt/math/TTwist3D.h line:19
		pybind11::class_<mrpt::math::TTwist3D, std::shared_ptr<mrpt::math::TTwist3D>> cl(M("mrpt::math"), "TTwist3D", "3D twist: 3D velocity vector (vx,vy,vz) + angular velocity (wx,wy,wz)\n \n\n mrpt::math::TTwist2D, mrpt::math::TPose3D\n \n\n\n ");
		cl.def( pybind11::init<double, double, double, double, double, double>(), pybind11::arg("vx_"), pybind11::arg("vy_"), pybind11::arg("vz_"), pybind11::arg("wx_"), pybind11::arg("wy_"), pybind11::arg("wz_") );

		cl.def( pybind11::init( [](){ return new mrpt::math::TTwist3D(); } ) );
		cl.def( pybind11::init( [](mrpt::math::TTwist3D const &o){ return new mrpt::math::TTwist3D(o); } ) );
		cl.def_readwrite("vx", &mrpt::math::TTwist3D::vx);
		cl.def_readwrite("vy", &mrpt::math::TTwist3D::vy);
		cl.def_readwrite("vz", &mrpt::math::TTwist3D::vz);
		cl.def_readwrite("wx", &mrpt::math::TTwist3D::wx);
		cl.def_readwrite("wy", &mrpt::math::TTwist3D::wy);
		cl.def_readwrite("wz", &mrpt::math::TTwist3D::wz);
		cl.def("__getitem__", (double & (mrpt::math::TTwist3D::*)(size_t)) &mrpt::math::TTwist3D::operator[], "Coordinate access using operator[]. Order: vx,vy,vz, wx, wy, wz \n\nC++: mrpt::math::TTwist3D::operator[](size_t) --> double &", pybind11::return_value_policy::automatic, pybind11::arg("i"));
		cl.def("__call__", (double & (mrpt::math::TTwist3D::*)(int, int)) &mrpt::math::TTwist3D::operator(), "(i,0) access operator (provided for API compatibility with matrices).\n \n\n operator[] \n\nC++: mrpt::math::TTwist3D::operator()(int, int) --> double &", pybind11::return_value_policy::automatic, pybind11::arg("row"), pybind11::arg("col"));
		cl.def("__imul__", (void (mrpt::math::TTwist3D::*)(const double)) &mrpt::math::TTwist3D::operator*=, "Scale factor \n\nC++: mrpt::math::TTwist3D::operator*=(const double) --> void", pybind11::arg("k"));
		cl.def("__eq__", (bool (mrpt::math::TTwist3D::*)(const struct mrpt::math::TTwist3D &) const) &mrpt::math::TTwist3D::operator==, "C++: mrpt::math::TTwist3D::operator==(const struct mrpt::math::TTwist3D &) const --> bool", pybind11::arg("o"));
		cl.def("__ne__", (bool (mrpt::math::TTwist3D::*)(const struct mrpt::math::TTwist3D &) const) &mrpt::math::TTwist3D::operator!=, "C++: mrpt::math::TTwist3D::operator!=(const struct mrpt::math::TTwist3D &) const --> bool", pybind11::arg("o"));
		cl.def("asString", (void (mrpt::math::TTwist3D::*)(std::string &) const) &mrpt::math::TTwist3D::asString, "Returns a human-readable textual representation of the object (eg: \"[vx\n vy vz wx wy wz]\", omegas in deg/s)\n \n\n fromString\n\nC++: mrpt::math::TTwist3D::asString(std::string &) const --> void", pybind11::arg("s"));
		cl.def("asString", (std::string (mrpt::math::TTwist3D::*)() const) &mrpt::math::TTwist3D::asString, "C++: mrpt::math::TTwist3D::asString() const --> std::string");
		cl.def("rotate", (void (mrpt::math::TTwist3D::*)(const struct mrpt::math::TPose3D &)) &mrpt::math::TTwist3D::rotate, "Transform all 6 components for a change of reference frame from \"A\" to\n another frame \"B\" whose rotation with respect to \"A\" is given by `rot`.\n The translational part of the pose is ignored \n\nC++: mrpt::math::TTwist3D::rotate(const struct mrpt::math::TPose3D &) --> void", pybind11::arg("rot"));
		cl.def("rotated", (struct mrpt::math::TTwist3D (mrpt::math::TTwist3D::*)(const struct mrpt::math::TPose3D &) const) &mrpt::math::TTwist3D::rotated, "Like rotate(), but returning a copy of the rotated twist.\n  \n\n New in MRPT 2.3.2 \n\nC++: mrpt::math::TTwist3D::rotated(const struct mrpt::math::TPose3D &) const --> struct mrpt::math::TTwist3D", pybind11::arg("rot"));
		cl.def("fromString", (void (mrpt::math::TTwist3D::*)(const std::string &)) &mrpt::math::TTwist3D::fromString, "Set the current object value from a string generated by 'asString' (eg:\n \"[vx vy vz wx wy wz]\" )\n \n\n asString\n \n\n std::exception On invalid format\n\nC++: mrpt::math::TTwist3D::fromString(const std::string &) --> void", pybind11::arg("s"));
		cl.def_static("FromString", (struct mrpt::math::TTwist3D (*)(const std::string &)) &mrpt::math::TTwist3D::FromString, "C++: mrpt::math::TTwist3D::FromString(const std::string &) --> struct mrpt::math::TTwist3D", pybind11::arg("s"));
	}
}
