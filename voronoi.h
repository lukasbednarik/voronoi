#ifndef VORONOI_H
#define VORONOI_H

#include "edge.h"
#include "event.h"
#include "beachline.h"
#include <vector>
#include <list>


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
		std::list<Voronoi::Edge> getEdges() const;

		
		// TODO get edges for one site function
		// TODO get next site in direction
		// TODO use std::reference_wrapper and compute with references... not to duplicate or round points


	private:
		std::vector<Point> _sites;   // Points for site event
		std::list<Edge> _edges;
		Beachline _beachline;
		std::list<Event> _eventQueue;
		std::list<Point> _tempVertex;  // Points for vertex event. TODO - tohle je problem, resime tim, ze nekdy Point nevlastnime, nekdy vlastnime...

		void _generate();
		void _processSiteEvent(const Event * event);
		void _processVertexEvent(Event * event);
		void _circleEvent(ParabolaNode * parabola, const double sweepline);
	};
}


#endif  // VORONOI_H
