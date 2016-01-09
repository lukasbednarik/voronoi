// The MIT License (MIT)
//
// Copyright (c) 2015 Lukáš Bednařík l.bednarik@gmail.com
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


#ifndef POINT_H
#define POINT_H

#include <cassert>


namespace Voronoi
{
	/// A structure that stores 2D point
	class Point
	{
	public:
		Point();
		Point(double x, double y);
		bool isNull() const;
		void setX(double x);
		void setY(double y);
		double x() const;
		double y() const;
		Point operator+(const Point & other) const;
		Point operator-(const Point & other) const;
		bool operator==(const Point & other) const;
		bool operator!=(const Point & other) const;
		bool operator<(const Point & other) const;
		bool operator>(const Point & other) const;

		template <typename T>
		Point operator*(const T number) const;

		template <typename T>
		Point operator/(const T number) const;

	private:
		bool _isNull;
		double _x;
		double _y;
	};
}


// Implementation

inline Voronoi::Point::Point() :
	_isNull(true),
	_x(0),
	_y(0)
{
}


inline Voronoi::Point::Point(double x, double y) :
	_isNull(false),
	_x(x),
	_y(y)
{
}


inline bool Voronoi::Point::isNull() const
{
	return _isNull;
}


inline void Voronoi::Point::setX(double x)
{
	_x = x;
}


inline void Voronoi::Point::setY(double y)
{
	_y = y;
}


inline double Voronoi::Point::x() const
{
	assert(!_isNull);
	return _x;
}


inline double Voronoi::Point::y() const
{
	assert(!_isNull);
	return _y;
}


inline Voronoi::Point Voronoi::Point::operator+(const Point & other) const
{
	return Point(_x + other._x, _y + other._y);
}


inline Voronoi::Point Voronoi::Point::operator-(const Point & other) const
{
	return Point(_x - other._x, _y - other._y);
}


inline bool Voronoi::Point::operator==(const Point & other) const
{
	return _x == other._x && _y == other._y;
}


inline bool Voronoi::Point::operator!=(const Point & other) const
{
	return !operator==(other);
}


inline bool Voronoi::Point::operator<(const Point & other) const
{
	if (_y == other._y) {
		// In this implementation the x-order is also important
		// Note: The x-order is important in `ProcessSite()` event where
		// some simplification is used based on this prerequisite.
		return _x > other._x;
	}
	else {
		return _y < other._y;
	}
}


inline bool Voronoi::Point::operator>(const Point & other) const
{
	if (_y == other._y) {
		// In this implementation the x-order is also important
		// Note: The x-order is important in `ProcessSite()` event where
		// some simplification is used based on this prerequisite.
		return _x < other._x;
	}
	else {
		return _y > other._y;
	}
}


template <typename T>
inline Voronoi::Point Voronoi::Point::operator*(const T number) const
{
	return Point(_x * number, _y * number);
}


template <typename T>
inline Voronoi::Point Voronoi::Point::operator/(const T number) const
{
	return Point(_x / number, _y / number);
}


#endif  // POINT_H
