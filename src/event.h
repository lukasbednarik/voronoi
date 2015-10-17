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


#ifndef EVENT_H
#define EVENT_H

#include "point.h"


namespace Voronoi
{
	class ParabolaNode;


	/// General event interface
	class Event
	{
	public:
		/// Constructor
		Event(const Point & site);

		/// Destructor
		virtual ~Event();

		/// True if this is a site event, false if this is a vertex event
		virtual bool isSiteEvent() const = 0;

		/// Return site associated with this event
		Point site() const;

		/// Disable this event
		void disable();

		/// Return true if this event should not be processed
		bool isDisabled() const;

	private:
		bool _isDisabled;
		Point _site;
	};


	class SiteEvent : public Event
	{
	public:
		SiteEvent(const Point & point);
		~SiteEvent() override;
		bool isSiteEvent() const override;
	};


	class VertexEvent : public Event
	{
	public:
		VertexEvent(const Point & point);
		~VertexEvent() override;
		bool isSiteEvent() const override;

		/// Set parabola for this event
		void setParabolaNode(ParabolaNode * parabolaNode);

		/// Get parabola for this event
		ParabolaNode * parabolaNode();

		/// Set circumcenter for this vertex event
		void setCircumcenter(const Point & circumcenter);

		/// Get circumcenter for this vertex event
		Point circumcenter() const;

	private:
		ParabolaNode * _parabolaNode;
		Point _circumcenter;
	};
}


// Implementation

inline Voronoi::Event::Event(const Point & site) :
	_isDisabled(false),
	_site(site)
{
}


inline Voronoi::Event::~Event()
{
}


inline bool Voronoi::Event::isDisabled() const
{
	return _isDisabled;
}


inline void Voronoi::Event::disable()
{
	_isDisabled = true;
}


inline Voronoi::Point Voronoi::Event::site() const
{
	return _site;
}


inline Voronoi::SiteEvent::SiteEvent(const Point & site) :
	Event(site)
{
}


inline Voronoi::SiteEvent::~SiteEvent()
{
}


inline bool Voronoi::SiteEvent::isSiteEvent() const
{
	return true;
}


inline Voronoi::VertexEvent::VertexEvent(const Point & site) :
	Event(site),
	_parabolaNode(nullptr)
{
}


inline Voronoi::VertexEvent::~VertexEvent()
{
}


inline void Voronoi::VertexEvent::setParabolaNode(ParabolaNode * parabolaNode)
{
	_parabolaNode = parabolaNode;
}


inline Voronoi::ParabolaNode * Voronoi::VertexEvent::parabolaNode()
{
	return _parabolaNode;
}


inline bool Voronoi::VertexEvent::isSiteEvent() const
{
	return false;
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
