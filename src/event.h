#ifndef EVENT_H
#define EVENT_H

#include "point.h"


namespace Voronoi
{
	class ParabolaNode;


	class Event
	{
	public:
		Event(const Point * point);

		virtual ~Event();

		const Point * site() const;

		/// True if this is a site event, false if this is a vertex event
		virtual bool isSiteEvent() const = 0;


		bool isDisabled() const;

		void disable();

		/// Sort by site->y() value
		bool operator>(const Event & other) const;

	private:
		const Point * _site;
		bool _isDisabled;
	};


	class SiteEvent : public Event
	{
	public:
		SiteEvent(const Point * point);
		~SiteEvent() override;
		bool isSiteEvent() const override;
	};


	class VertexEvent : public Event
	{
	public:
		VertexEvent(const Point * point);
		~VertexEvent() override;

		bool isSiteEvent() const override;


		/// Set parabola for this event
		void setParabolaNode(ParabolaNode * parabolaNode);

		/// Get parabola for this event
		ParabolaNode * parabolaNode();

		Point circumcenter;  // TODO (muze byt null)
	private:
		ParabolaNode * _parabolaNode;
	};
}


// Implementation

inline Voronoi::Event::Event(const Point * site) :
	_site(site),
	_isDisabled(false)
{
}


inline Voronoi::Event::~Event()
{
}


inline const Voronoi::Point * Voronoi::Event::site() const
{
	return _site;
}


inline bool Voronoi::Event::isDisabled() const
{
	return _isDisabled;
}


inline void Voronoi::Event::disable()
{
	_isDisabled = true;
}


inline bool Voronoi::Event::operator>(const Event & other) const
{
	return _site->y() > other.site()->y();
}


inline Voronoi::SiteEvent::SiteEvent(const Point * site) :
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


inline Voronoi::VertexEvent::VertexEvent(const Point * site) :
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


#endif  // EVENT_H
