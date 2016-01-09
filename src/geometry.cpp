#include "geometry.h"
#include "make_unique.h"
#include <algorithm>
#include <cmath>
#include <cassert>


namespace
{
	const double EpsilonRad = 1e-10;

	/// Return true if a given angle in rad is zero
	inline bool isZeroAngle(double angle)
	{
		return angle < EpsilonRad && -angle < EpsilonRad;
	}
}


Voronoi::Point Voronoi::edgeIntersection(const Edge & left, const Edge & right)
{
	// Suppose we have lines given by {(x_1, y_1), (x_2, y_2)} and {(x_3, y_3), (x_4, y_4)}.
	const double x_1 = left.begin().x();
	const double y_1 = left.begin().y();
	const double x_2 = left.end().x();
	const double y_2 = left.end().y();
	const double x_3 = right.begin().x();
	const double y_3 = right.begin().y();
	const double x_4 = right.end().x();
	const double y_4 = right.end().y();

	const double denominator = (y_4 - y_3) * (x_2 - x_1) - (x_4 - x_3) * (y_2 - y_1);
	const double numerator1 = (x_4 - x_3) * (y_1 - y_3) - (y_4 - y_3) * (x_1 - x_3);
	const double numerator2 = (x_2 - x_1) * (y_1 - y_3) - (y_2 - y_1) * (x_1 - x_3);

	if (isZeroAngle(denominator) && isZeroAngle(numerator1) &&
			isZeroAngle(numerator2)) {
		// The two lines are coincidents
		return Point();
	}
	if (isZeroAngle(denominator)) {
		// The line segments are parallel
		return Point();
	}
	double factor1 = numerator1 / denominator;
	double factor2 = numerator2 / denominator;
	if (factor1 < 0.0 || factor1 > 1.0 || factor2 < 0.0 || factor2 > 1.0) {
		// The intersection is outside the line segments, line segments don't collide
		// Note: We need to check both factors here!
		return Point();
	}
	// Line segments intersect
	double x = x_1 + factor1 * (x_2 - x_1);
	double y = y_1 + factor1 * (y_2 - y_1);
	return Point(x, y);
}


Voronoi::Point Voronoi::circumcenter(const Point & a, const Point & b, const Point & c)
{
	// This equation can be expressed in a simplified form after translation of the vertex A to the origin
	// of the Cartesian coordinate systems. But in that case the equation wouldn't be symetric
	// (which is be bad for numerical computations).
	auto norm2 = [] (const Point & p) -> double { return p.x() * p.x() + p.y() * p.y(); };
	const double x = norm2(a) * (b.y() - c.y()) + norm2(b) * (c.y() - a.y()) + norm2(c) * (a.y() - b.y());
	const double y = norm2(a) * (c.x() - b.x()) + norm2(b) * (a.x() - c.x()) + norm2(c) * (b.x() - a.x());
	const double d = 2 * (a.x() * (b.y() - c.y()) + b.x() * (c.y() - a.y()) + c.x() * (a.y() - b.y()));
	if (isZeroAngle(d)) {
		return Point();
	}
	else {
		return Point(x / d, y / d);
	}
}


double Voronoi::circumcircleRadius(const Point & a, const Point & b, const Point & c)
{
	auto norm = [] (const Point & p) -> double { return std::sqrt(p.x() * p.x() + p.y() * p.y()); };

	// Compute side lengths
	const double e = norm(a - b);
	const double f = norm(b - c);
	const double g = norm(c - a);
	const double s = (e + f + g) / 2.0;

	// Compute divisor
	const double d = std::sqrt(s * (s - e) * (s - f) * (s - g));

	// Use formula
	if (isZeroAngle(d)) {
		return 0;
	}
	else {
		return e * f * g / 4.0 / d;
	}
}


double Voronoi::circleRadius(const Point & center, const Point & x)
{
	const double dx = center.x() - x.x();
	const double dy = center.y() - x.y();
	return std::sqrt(dx * dx + dy * dy);
}


double Voronoi::parabolaIntersectionX(const Point & leftParabola, const Point & rightParabola, double directrix)
{
	const Point & p = leftParabola;
	const Point & r = rightParabola;
	const double & y = directrix;
	const Point diff = p - r;
	assert(!isZeroAngle(diff.x()) || !isZeroAngle(diff.y()));
	assert(!isZeroAngle(r.y() - y));  // We suppose parabola is not degenerate
	assert(!isZeroAngle(p.y() - y));  // We suppose parabola is not degenerate

	// Coefficients of the first parabola
	// a1 * x * x + b1 * x + c1 = 0
	const double dp1 = 2.0 * (p.y() - y);
	const double a1 = 1.0 / dp1;
	const double b1 = -2.0 * p.x() / dp1;
	const double c1 = y + dp1 / 4 + p.x() * p.x() / dp1;

	// Coefficients of the second parabola
	// a2 * x * x + b2 * x + c2 = 0
	const double dp2 = 2.0 * (r.y() - y);
	const double a2 = 1.0 / dp2;
	const double b2 = -2.0 * r.x() / dp2;
	const double c2 = y + dp2 / 4 + r.x() * r.x() / dp2;

	// Coefficients for difference of two parabolas
	const double a = a1 - a2;
	const double b = b1 - b2;
	const double c = c1 - c2;

	// Degenerate case when foci of parabolas have the same `y` coordinate.
	if (isZeroAngle(a)) {
		return -c / b;
	}

	// Solve a quadratic equation when the difference of two parabolas is zero.
	// This is when the two parabolas intersects.
	const double disc = b*b - 4 * a * c;
	assert(disc >= 0);  // We suppose parabolas intersect
	const double x1 = (-b + std::sqrt(disc)) / (2*a);
	const double x2 = (-b - std::sqrt(disc)) / (2*a);

	// Suppose parabola "p" is on the left.
	// Check if the line will go up or down and decide which value to return.
	if (p.y() < r.y()) {
		return std::max(x1, x2);
	}
	else {
		return std::min(x1, x2);
	}	
}


double Voronoi::getParabolaY(Voronoi::Point focus, double directrix, double x)
{
	const double dp = 2.0 * (focus.y() - directrix);
	if (isZeroAngle(dp)) {
		// Degenerated parabola is a line
		return directrix;
	}
	const double a = 1.0 / dp;
	const double b = -2.0 * focus.x() / dp;
	const double c = directrix + dp / 4.0 + focus.x() * focus.x() / dp;
	return a * x * x + b * x + c;
}

