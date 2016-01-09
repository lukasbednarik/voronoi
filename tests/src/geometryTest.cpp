#include "tests.h"
#include "geometry.h"
#include "edge.h"

using namespace Voronoi;

static const double Epsilon = 1e-6;


SUITE(GeometryTest)
{
	TEST(EdgeIntersection)
	{
		const Point dummy(0.0, 0.0);
		Edge left(dummy, dummy);
		Edge right(dummy, dummy);
		left.setBegin(Point(0, 1));
		left.setEnd(Point(0, 5));
		right.setBegin(Point(-1, 3));
		right.setEnd(Point(1, 3));

		auto intersection = edgeIntersection(left, right);
		CHECK_CLOSE(0.0, intersection.x(), Epsilon);
		CHECK_CLOSE(3.0, intersection.y(), Epsilon);
	}


	TEST(EdgeIntersection_NoIntersection)
	{
		const Point dummy(0.0, 0.0);
		Edge left(dummy, dummy);
		Edge right(dummy, dummy);
		left.setBegin(Point(0, 1));
		left.setEnd(Point(0, 5));
		right.setBegin(Point(-1, 10));
		right.setEnd(Point(1, 10));

		auto intersection = edgeIntersection(left, right);
		CHECK(intersection.isNull());
	}


	TEST(Circumcenter_010224_Correct)
	{
		auto center = circumcenter(Point(0, 1), Point(0, 2), Point(2, 4));
		CHECK_CLOSE(2.5, center.x(), Epsilon);
		CHECK_CLOSE(1.5, center.y(), Epsilon);
	}


	TEST(Circumcenter_240201_Correct)
	{
		auto center = circumcenter(Point(2, 4), Point(0, 2), Point(0, 1));
		CHECK_CLOSE(2.5, center.x(), Epsilon);
		CHECK_CLOSE(1.5, center.y(), Epsilon);
	}


	TEST(Circumcenter_240102_Correct)
	{
		auto center = circumcenter(Point(2, 4), Point(0, 1), Point(0, 2));
		CHECK_CLOSE(2.5, center.x(), Epsilon);
		CHECK_CLOSE(1.5, center.y(), Epsilon);
	}


	TEST(Circumcenter_010102_NoResult)
	{
		auto center = circumcenter(Point(0, 1), Point(0, 1), Point(0, 2));
		CHECK(center.isNull());
	}


	TEST(CircumcircleRadius_010224_Correct)
	{
		auto radius = circumcircleRadius(Point(0, 1), Point(0, 2), Point(2, 4));
		CHECK_CLOSE(2.549509756796, radius, Epsilon);
	}


	TEST(CircumcircleRadius_240201_Correct)
	{
		auto radius = circumcircleRadius(Point(2, 4), Point(0, 2), Point(0, 1));
		CHECK_CLOSE(2.549509756796, radius, Epsilon);
	}


	TEST(CircumcircleRadius_240102_Correct)
	{
		auto radius = circumcircleRadius(Point(2, 4), Point(0, 1), Point(0, 2));
		CHECK_CLOSE(2.549509756796, radius, Epsilon);
	}


	TEST(CircumcircleRadius_010102_NoResult)
	{
		auto radius = circumcircleRadius(Point(0, 1), Point(0, 1), Point(0, 2));
		CHECK_CLOSE(0.0, radius, Epsilon);
	}


	TEST(CircleRadius_0107_6)
	{
		auto radius = circleRadius(Point(0, 1), Point(0, 7));
		CHECK_CLOSE(6.0, radius, Epsilon);
	}


	TEST(CircleRadius_1070_8)
	{
		auto radius = circleRadius(Point(-1, 0), Point(7, 0));
		CHECK_CLOSE(8.0, radius, Epsilon);
	}


	TEST(CircleRadius_0101_0)
	{
		auto radius = circleRadius(Point(0, 1), Point(0, 1));
		CHECK_CLOSE(0.0, radius, Epsilon);
	}


	TEST(GetParabolaY_1101_05)
	{
		double y = getParabolaY(Point(1, 1), 0, 1);
		CHECK_CLOSE(0.5, y, Epsilon);
	}


	TEST(GetParabolaY_0001_0)
	{
		double y = getParabolaY(Point(0, 0), 0, 1);
		CHECK_CLOSE(0.0, y, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_1)
	{
		double x = parabolaIntersectionX(Point(0, 0), Point(1, 1), -1);
		CHECK_CLOSE(1.0, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineOver_1)
	{
		double x = parabolaIntersectionX(Point(0, 0), Point(1, 1), 2);
		CHECK_CLOSE(4.0, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_2)
	{
		double x = parabolaIntersectionX(Point(0, -1), Point(0, 1), -5);
		CHECK_CLOSE(4.8989794855663, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_3)
	{
		double x = parabolaIntersectionX(Point(0, 1), Point(0, -1), -5);
		CHECK_CLOSE(-4.8989794855663, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_4)
	{
		double x = parabolaIntersectionX(Point(-1, 0), Point(1, 0), -5);
		CHECK_CLOSE(0.0, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_5)
	{
		double x = parabolaIntersectionX(Point(-1, 0), Point(1, 0), -5);
		CHECK_CLOSE(0.0, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_6)
	{
		double x = parabolaIntersectionX(Point(1, -1), Point(0, 1), -5);
		CHECK_CLOSE(8.477225575051, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_7)
	{
		double x = parabolaIntersectionX(Point(1, 1), Point(0, -1), -5);
		CHECK_CLOSE(-7.47722557505, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_8)
	{
		double x = parabolaIntersectionX(Point(-1, 1), Point(1, 0), -5);
		CHECK_CLOSE(-1.247448713915, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_9)
	{
		double x = parabolaIntersectionX(Point(-1, 1), Point(1, 0), -5);
		CHECK_CLOSE(-1.247448713915, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_10)
	{
		double x = parabolaIntersectionX(Point(0, -1), Point(1, 1), -5);
		CHECK_CLOSE(3.477225575051, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_11)
	{
		double x = parabolaIntersectionX(Point(0, 1), Point(1, -1), -5);
		CHECK_CLOSE(-2.47722557505, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_12)
	{
		double x = parabolaIntersectionX(Point(-1, 0), Point(1, 1), -5);
		CHECK_CLOSE(1.247448713915, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_LineUnder_13)
	{
		double x = parabolaIntersectionX(Point(-1, 0), Point(1, 1), -5);
		CHECK_CLOSE(1.2474487139158, x, Epsilon);
	}


	TEST(ParabolaIntersectionX_Universal_X)
	{
		const Point left(1, -1);
		const Point right(0, 1);
		const double directrix = -5;
		double x = parabolaIntersectionX(left, right, directrix);
		double y = getParabolaY(left, directrix, x);
		auto norm = [](const Point & p) -> double { return std::sqrt(p.x() * p.x() + p.y() * p.y()); };
		CHECK_CLOSE(norm(Point(x, y) - left), norm(Point(x, y) - right), Epsilon);
	}

}
