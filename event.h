#ifndef EVENT_H
#define EVENT_H

#include "point.h"


namespace Voronoi
{
	class ParabolaNode;


	class Event
	{
	public:
		Event(const Point * point, bool isSiteEvent = true);

		const Point * site() const;

		/// True if this is a site event, false if this is a vertex event
		bool isSiteEvent() const;

		/// Set parabola for this event
		void setParabolaNode(ParabolaNode * parabolaNode);

		/// Get parabola for this event
		ParabolaNode * parabolaNode();

		bool isDisabled() const;

		void disable();

	private:
		const Point * _site;
		bool _isSiteEvent;
		ParabolaNode * _parabolaNode;
		bool _isDisabled;
	};
}


// Implementation

inline Voronoi::Event::Event(const Point * site, bool isSiteEvent) :
	_site(site),
	_isSiteEvent(isSiteEvent),
	_parabolaNode(nullptr),
	_isDisabled(false)
{
}


inline const Voronoi::Point * Voronoi::Event::site() const
{
	return _site;
}


inline bool Voronoi::Event::isSiteEvent() const
{
	return _isSiteEvent;
}


inline void Voronoi::Event::setParabolaNode(ParabolaNode * parabolaNode)
{
	_parabolaNode = parabolaNode;
}


inline Voronoi::ParabolaNode * Voronoi::Event::parabolaNode()
{
	return _parabolaNode;
}


inline bool Voronoi::Event::isDisabled() const
{
	return _isDisabled;
}


inline void Voronoi::Event::disable()
{
	_isDisabled = true;
}


#endif  // EVENT_H
