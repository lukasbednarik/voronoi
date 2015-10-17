// The MIT License (MIT)
//
// Copyright (c) 2015 Lukáš Bednařík
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


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
