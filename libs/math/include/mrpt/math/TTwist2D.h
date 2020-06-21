/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2020, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */
#pragma once

#include <mrpt/math/TPoseOrPoint.h>
#include <vector>

namespace mrpt::math
{
/** 2D twist: 2D velocity vector (vx,vy) + planar angular velocity (omega)
 * \sa mrpt::math::TTwist3D, mrpt::math::TPose2D
 */
struct TTwist2D : public internal::ProvideStaticResize<TTwist2D>
{
	enum
	{
		static_size = 3
	};
	/** Velocity components: X,Y (m/s) */
	double vx{.0}, vy{.0};
	/** Angular velocity (rad/s) */
	double omega{.0};

	/** Constructor from components */
	constexpr TTwist2D(double vx_, double vy_, double omega_)
		: vx(vx_), vy(vy_), omega(omega_)
	{
	}
	/** Default fast constructor. Initializes to zeros  */
	TTwist2D() = default;
	/** Coordinate access using operator[]. Order: vx,vy,vphi */
	double& operator[](size_t i)
	{
		switch (i)
		{
			case 0:
				return vx;
			case 1:
				return vy;
			case 2:
				return omega;
			default:
				throw std::out_of_range("index out of range");
		}
	}
	/** Coordinate access using operator[]. Order: vx,vy,vphi */
	constexpr double operator[](size_t i) const
	{
		switch (i)
		{
			case 0:
				return vx;
			case 1:
				return vy;
			case 2:
				return omega;
			default:
				throw std::out_of_range("index out of range");
		}
	}
	/** Transformation into vector */
	void asVector(std::vector<double>& v) const
	{
		v.resize(3);
		v[0] = vx;
		v[1] = vy;
		v[2] = omega;
	}
	/** Transform the (vx,vy) components for a counterclockwise rotation of
	 * `ang` radians. */
	void rotate(const double ang);
	bool operator==(const TTwist2D& o) const;
	bool operator!=(const TTwist2D& o) const;
	/** Returns the pose increment of multiplying each twist component times
	 * "dt" seconds. */
	mrpt::math::TPose2D operator*(const double dt) const;

	/** Scale factor */
	void operator*=(const double k)
	{
		vx *= k;
		vy *= k;
		omega *= k;
	}

	/** Returns a human-readable textual representation of the object (eg: "[vx
	 * vy omega]", omega in deg/s)
	 * \sa fromString
	 */
	void asString(std::string& s) const;
	std::string asString() const
	{
		std::string s;
		asString(s);
		return s;
	}

	/** Set the current object value from a string generated by 'asString' (eg:
	 * "[0.02 1.04 -45.0]" )
	 * \sa asString
	 * \exception std::exception On invalid format
	 */
	void fromString(const std::string& s);

	static TTwist2D FromString(const std::string& s)
	{
		TTwist2D o;
		o.fromString(s);
		return o;
	}
};

mrpt::serialization::CArchive& operator>>(
	mrpt::serialization::CArchive& in, mrpt::math::TTwist2D& o);
mrpt::serialization::CArchive& operator<<(
	mrpt::serialization::CArchive& out, const mrpt::math::TTwist2D& o);

}  // namespace mrpt::math

namespace mrpt::typemeta
{
// Specialization must occur in the same namespace
MRPT_DECLARE_TTYPENAME_NO_NAMESPACE(TTwist2D, mrpt::math)

}  // namespace mrpt::typemeta
