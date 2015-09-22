#ifndef VORONOI_H
#define VORONOI_H

#include "edge.h"
#include "event.h"
#include "beachline.h"
#include <vector>
#include <list>
#include <utility>


namespace Voronoi
{
	class Generator
	{
	public:
		/// Calculate Voronoi diagram
		///
		/// @param sites Sites in the [0, 1] interval.
		Generator(const std::vector<Point> & sites);

		/// Return all edges of Voronoi diagram
		std::vector<Voronoi::Edge> getEdges() const;

		
		// TODO get edges for one site function
		// TODO get next site in direction
		// TODO use std::reference_wrapper and compute with references... not to duplicate or round points


	private:
		typedef std::pair<std::unique_ptr<Event>, std::unique_ptr<Event>> CircleEventType;

		std::vector<Point> _sites;   // Points for site event
		std::list<Point> _vertices;  // Points for vertex event
		std::vector<Edge> _edges;
		Beachline _beachline;

		void _findEdges();
		CircleEventType _processSiteEvent(const Event * event);
		CircleEventType  _processVertexEvent(Event * event);
		std::unique_ptr<Event> _circleEvent(ParabolaNode * parabola, const double sweepline);
	};
}


#endif  // VORONOI_H
