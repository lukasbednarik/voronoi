#ifndef EDGE_H
#define EDGE_H

#include "point.h"
#include <memory>


namespace Voronoi
{
	/// Edge class stores an edge in Voronoi diagram
	class Edge
	{
	public:
		Edge();
		Edge(const Point * left, const Point * right);
		const Point * begin() const;
		const Point * end() const;
		const Point * left() const;
		const Point * right() const;
		void setBegin(const Point * begin);
		void setEnd(const Point * end);
		void setLeft(const Point * left);
		void setRight(const Point * right);

	private:
		const Point * _begin;  ///< Start of the edge
		const Point * _end;    ///< End of the edge
		const Point * _left;   ///< Each edge lies between two points, this is the left
		const Point * _right;  ///< Each edge lies between two points, this is the right
	};
}


// Implementation

inline Voronoi::Edge::Edge() :
	_begin(nullptr),
	_end(nullptr),
	_left(nullptr),
	_right(nullptr)
{
}
	

inline Voronoi::Edge::Edge(const Point * left, const Point * right) :
	_begin(nullptr),
	_end(nullptr),
	_left(left),
	_right(right)
{
}


inline const Voronoi::Point * Voronoi::Edge::begin() const
{
	return _begin;
}


inline const Voronoi::Point * Voronoi::Edge::end() const
{
	return _end;
}


inline const Voronoi::Point * Voronoi::Edge::left() const
{
	return _left;
}


inline const Voronoi::Point * Voronoi::Edge::right() const
{
	return _right;
}


inline void Voronoi::Edge::setBegin(const Point * begin)
{
	_begin = begin;
}


inline void Voronoi::Edge::setEnd(const Point * end)
{
	_end = end;
}


inline void Voronoi::Edge::setLeft(const Point * left)
{
	_left = left;
}


inline void Voronoi::Edge::setRight(const Point * right)
{
	_right = right;
}

#endif  // EDGE_H
