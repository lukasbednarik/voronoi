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
		Edge(const Point * left, const Point * right);
		const Point * begin() const;   // TODO Az bude algoritmus fungovat, zmenit na vraceni hodnoty. std::pair lze po dukladnem odladeni take odstranit a vyhazovat jen vyjimku pro kontrolu, pokud uzivatel pozada o nenastavenou hodnotu.
		const Point * end() const;
		const Point * left() const;
		const Point * right() const;
		void setBegin(const Point & begin);
		void setEnd(const Point & end);
		bool operator<(const Edge & other) const;
		bool operator==(const Edge & other) const;

		Edge * neighbour;  ///< some edges consist of two parts, so we add the pointer to another part to connect them at the end of an algorithm

	private:
		std::pair<bool, Point> _begin;  ///< Start of the edge
		std::pair<bool, Point> _end;    ///< End of the edge
		const Point * _left;   ///< Each edge lies between two points, this is the left
		const Point * _right;  ///< Each edge lies between two points, this is the right
	};
}


// Implementation

inline Voronoi::Edge::Edge(const Point * left, const Point * right) :
	_begin(std::make_pair(false, Point())),
	_end(std::make_pair(false, Point())),
	_left(left),
	_right(right)
{
}


inline const Voronoi::Point * Voronoi::Edge::begin() const
{
	if (_begin.first) {
		return &_begin.second;
	}
	else {
		return nullptr;
	}
}


inline const Voronoi::Point * Voronoi::Edge::end() const
{
	if (_end.first) {
		return &_end.second;
	}
	else {
		return nullptr;
	}
}


inline const Voronoi::Point * Voronoi::Edge::left() const
{
	return _left;
}


inline const Voronoi::Point * Voronoi::Edge::right() const
{
	return _right;
}


inline void Voronoi::Edge::setBegin(const Point & begin)
{
	_begin = std::make_pair(true, begin);
}


inline void Voronoi::Edge::setEnd(const Point & end)
{
	_end = std::make_pair(true, end);
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
