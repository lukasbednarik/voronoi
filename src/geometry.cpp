#include "geometry.h"
#include "make_unique.h"
#include <algorithm>
#include <cmath>
#include <cassert>


namespace
{
const double EpsilonRad = 1e-10;

/// Return true if a given angle in rad is zero
inline bool IsZeroAngle(double angle)
{
	return angle < EpsilonRad && -angle < EpsilonRad;
}
}


std::unique_ptr<Voronoi::Point> Voronoi::EdgeIntersection(const Edge & left, const Edge & right)
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

	if (IsZeroAngle(denominator) && IsZeroAngle(numerator1) &&
			IsZeroAngle(numerator2)) {
		// The two lines are coincidents
		return nullptr;
	}
	if (IsZeroAngle(denominator)) {
		// The line segments are parallel
		return nullptr;
	}
	double factor1 = numerator1 / denominator;
	double factor2 = numerator2 / denominator;
	if (factor1 < 0.0 || factor1 > 1.0 || factor2 < 0.0 || factor2 > 1.0) {
		// The intersection is outside the line segments, line segments don't collide
		// Note: We need to check both factors here!
		return nullptr;
	}
	// Line segments intersect
	// x = x_1 + factor1 * (x_2 - x_1) 
	double y = y_1 + factor1 * (y_2 - y_1);
	// y = y_1 + factor1 * (y_2 - y_1) 
	double x = x_1 + factor1 * (x_2 - x_1);

	return make_unique<Voronoi::Point>(x, y);
}


std::unique_ptr<Voronoi::Point> Voronoi::Circumcenter(const Point & a, const Point & b, const Point & c)
{
	auto norm2 = [] (const Point & p) -> double { return p.x() * p.x() + p.y() * p.y(); };

	// Translate "a" into origin
	const Point beta = b - a;
	const Point gamma = c - a;

	// Compute x, y
	const double determinant = 2 * (beta.x() * gamma.y() - gamma.x() * beta.y());
	if (IsZeroAngle(determinant)) {
		return nullptr;
	}

	const double x = gamma.y() * norm2(beta) - beta.y() * norm2(gamma);
	const double y = beta.x() * norm2(gamma) - gamma.x() * norm2(beta);
	const double c_x = x / determinant + a.x();
	const double c_y = y / determinant + a.y();
	return make_unique<Point>(c_x, c_y);
}


double Voronoi::CircumcircleRadius(const Point & a, const Point & b, const Point & c)
{
	auto norm = [] (const Point & p) -> double { return std::sqrt(p.x() * p.x() + p.y() * p.y()); };

	// Compute side lengths
	const double e = norm(a - b);
	const double f = norm(b - c);
	const double g = norm(c - a);

	// Use formula
	const double s = (e + f + g) / 2;
	double radius = e * f * g;
	radius /= 4 * std::sqrt(s * (s - e) * (s - f) * (s - g));
	return radius;
}


double Voronoi::CircleRadius(const Point & center, const Point & x)
{
	const double dx = center.x() - x.x();
	const double dy = center.y() - x.y();

	return std::sqrt(dx * dx + dy * dy);
}


double Voronoi::parabolaIntersectionX(const Point & p, const Point & r, double y)
{
	assert(p != r); // if true, then b1 != b2 which imply b != 0

	// Coefficients of the first parabola
	// a1 * x * x + b1 * x + c1 = 0
	const double dp1 = 2.0 * (p.y() - y);
	const double a1 = 1.0 / dp1;
	const double b1 = -2.0 * p.x() / dp1;
	const double c1 = y + dp1 / 4 + p.x() * p.x() / dp1;

	// Coefficients of the second parabola
	// a1 * x * x + b1 * x + c1 = 0
	const double dp2 = 2.0 * (r.y() - y);
	const double a2 = 1.0 / dp2;
	const double b2 = -2.0 * r.x() / dp2;
	const double c2 = y + dp2 / 4 + r.x() * r.x() / dp2;

	// Coefficients for difference of two parabolas
	const double a = a1 - a2;
	const double b = b1 - b2;
	const double c = c1 - c2;

	// Degenerate cast
	if (a == 0) {
		return -c / b;
	}

	// Solve a quadratic equation when the difference of two parabolas is zero.
	// This is when the two parabolas intersects.
	const double disc = b*b - 4 * a * c;
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
	const double dp = 2 * (focus.y() - directrix);
	const double a1 = 1 / dp;
	const double b1 = -2 * focus.x() / dp;
	const double c1 = directrix + dp / 4 + focus.x() * focus.x() / dp;
	return a1 * x * x + b1 * x + c1;
}

