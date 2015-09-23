#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "point.h"
#include "edge.h"
#include <memory>


namespace Voronoi
{
	// POZN: Intersekce neni potreba pro generovani voronoi, ale bude potreba pozdeji.

	/// Return an intersection of two edges or nullptr if no intersection
	std::unique_ptr<Voronoi::Point> EdgeIntersection(const Edge & left, const Edge & right);

	/// Circumcenter of three points
	std::unique_ptr<Point> Circumcenter(const Point & a, const Point & b, const Point & c);

	/// Circumcircle radius of three points
	double CircumcircleRadius(const Point & a, const Point & b, const Point & c);

	/// Calculate circle radius given the center and one point on the circle
	double CircleRadius(const Point & center, const Point & x);

	/// Let's have a parabola defined by a focus and a directrix.
	/// Find "y" value for given "x".
	double getParabolaY(Point focus, double directrix, double x);

	/// Retrun "x" coordinate of two parabola intersection.
	double parabolaIntersectionX(const Point & p, const Point & r, double y);
}


#endif  // GEOMETRY_H
