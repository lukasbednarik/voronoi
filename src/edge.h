#ifndef EDGE_H
#define EDGE_H

#include "point.h"
#include <utility>


namespace Voronoi
{
	/// Edge class stores an edge in Voronoi diagram
	class Edge
	{
	public:
		Edge(const Point & left, const Point & right);
		Point begin() const;
		Point end() const;
		Point left() const;
		Point right() const;
		void setBegin(const Point & begin);
		void setEnd(const Point & end);
		bool operator<(const Edge & other) const;
		bool operator==(const Edge & other) const;

		Edge * neighbour;  ///< some edges consist of two parts, so we add the pointer to another part to connect them at the end of an algorithm

	private:
		Point _begin;  ///< Start of the edge
		Point _end;    ///< End of the edge
		Point _left;   ///< Each edge lies between two points, this is the left
		Point _right;  ///< Each edge lies between two points, this is the right
	};
}


// Implementation

inline Voronoi::Edge::Edge(const Point & left, const Point & right) :
	_left(left),
	_right(right),
	neighbour(nullptr)
{
}


inline Voronoi::Point Voronoi::Edge::begin() const
{
	return _begin;
}


inline Voronoi::Point Voronoi::Edge::end() const
{
	return _end;
}


inline Voronoi::Point Voronoi::Edge::left() const
{
	return _left;
}


inline Voronoi::Point Voronoi::Edge::right() const
{
	return _right;
}


inline void Voronoi::Edge::setBegin(const Point & begin)
{
	_begin = begin;
}


inline void Voronoi::Edge::setEnd(const Point & end)
{
	_end = end;
}


inline bool Voronoi::Edge::operator<(const Edge & other) const
{
	if (_begin != other._begin) {
		return _begin < other._begin;
	}
	else {
		return _end < other._end;
	}
}


inline bool Voronoi::Edge::operator==(const Edge & other) const
{
	return _begin == other._begin && _end == other._end;
}


#endif  // EDGE_H
