#ifndef VORONOI_H
#define VORONOI_H

#include "edge.h"
#include "event.h"
#include "beachline.h"
#include <vector>
#include <list>
#include <queue>


namespace Voronoi
{

	static const auto compareEvents = [] (const std::unique_ptr<Event> & left, const std::unique_ptr<Event> & right) -> bool
	{
		return left->site().y() < right->site().y();
	};


	class Generator
	{
	public:
		/// Calculate Voronoi diagram
		///
		/// @param sites Sites in the [0, 1] interval.
		Generator(const std::vector<Point> & sites);

		/// Return all edges of Voronoi diagram
		std::list<Edge> getEdges() const;

		
		// TODO get edges for one site function
		// TODO get next site in direction
		// TODO use std::reference_wrapper and compute with references... not to duplicate or round points


	private:
		std::list<Edge> _edges;
		Beachline _beachline;
		std::priority_queue<std::unique_ptr<Event>, std::vector<std::unique_ptr<Event>>, decltype(compareEvents)> _eventQueue;

		void _generate();
		void _processEvent(const SiteEvent * event);
		void _processEvent(VertexEvent * event);
		void _circleEvent(ParabolaNode * parabola, const double sweepline);
	};
}


#endif  // VORONOI_H
