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


#ifndef EVENT_H
#define EVENT_H

#include "point.h"


namespace Voronoi
{
	class ParabolaNode;


	/// Simple site event triggered by input point
	class SiteEvent
	{
	public:
		SiteEvent(const Point & site);
		bool operator>(const SiteEvent & other) const;
		Point site() const;
	private:
		Point _site;
	};


	/// Generated vertex event at curcumcircle of three sites
	class VertexEvent
	{
	public:
		VertexEvent(const Point & site);

		Point site() const;

		/// Set parabola for this event
		void setParabolaNode(ParabolaNode * parabolaNode);

		/// Get parabola for this event
		ParabolaNode * parabolaNode();

		/// Set circumcenter for this vertex event
		void setCircumcenter(const Point & circumcenter);

		/// Get circumcenter for this vertex event
		Point circumcenter() const;

		/// Disable this event
		void disable();

		/// Return true if this event should not be processed
		bool isDisabled() const;

	private:
		Point _site;
		bool _isDisabled;
		ParabolaNode * _parabolaNode;
		Point _circumcenter;
	};


	inline bool operator>(const SiteEvent & left, const SiteEvent & right) { return left.operator>(right); }
}


// Implementation
inline Voronoi::SiteEvent::SiteEvent(const Point & site) :
	_site(site)
{
}


inline bool Voronoi::SiteEvent::operator>(const SiteEvent & other) const
{
	return _site > other._site;
}


inline Voronoi::Point Voronoi::SiteEvent::site() const
{
	return _site;
}


inline Voronoi::VertexEvent::VertexEvent(const Point & site) :
	_site(site),
	_parabolaNode(nullptr),
	_isDisabled(false)
{
}


inline Voronoi::Point Voronoi::VertexEvent::site() const
{
	return _site;
}


inline bool Voronoi::VertexEvent::isDisabled() const
{
	return _isDisabled;
}


inline void Voronoi::VertexEvent::disable()
{
	_isDisabled = true;
}


inline void Voronoi::VertexEvent::setParabolaNode(ParabolaNode * parabolaNode)
{
	_parabolaNode = parabolaNode;
}


inline Voronoi::ParabolaNode * Voronoi::VertexEvent::parabolaNode()
{
	return _parabolaNode;
}


inline void Voronoi::VertexEvent::setCircumcenter(const Point & circumcenter)
{
	_circumcenter = circumcenter;
}


inline Voronoi::Point Voronoi::VertexEvent::circumcenter() const
{
	return _circumcenter;
}


#endif  // EVENT_H
