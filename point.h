#ifndef POINT_H
#define POINT_H


namespace Voronoi
{
	/// A structure that stores 2D point
	class Point
	{
	public:
		Point();
		Point(double x, double y);
		void setX(double x);
		void setY(double y);
		double x() const;
		double y() const;
		Point operator+(const Point & other) const;
		Point operator-(const Point & other) const;
		bool operator==(const Point & other) const;
		bool operator!=(const Point & other) const;

		template <typename T>
		Point operator*(const T number) const;

		template <typename T>
		Point operator/(const T number) const;

	private:
		double _x;
		double _y;
	};
}


// Implementation

inline Voronoi::Point::Point() :
_x(0),
	_y(0)
{
}


inline Voronoi::Point::Point(double x, double y) :
_x(x),
	_y(y)
{
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
	return _x;
}


inline double Voronoi::Point::y() const
{
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
