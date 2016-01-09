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
	struct BoundingBox
	{
		BoundingBox() : MinX(0), MaxX(1), MinY(0), MaxY(1) {}
		BoundingBox(double minX, double maxX, double minY, double maxY);
		double MinX;
		double MaxX;
		double MinY;
		double MaxY;
	};


	class Generator
	{
	public:
		/// Calculate Voronoi diagram
		Generator(const std::vector<Point> & sites, const BoundingBox & boundingBox = BoundingBox());

		/// Return all edges of Voronoi diagram
		std::list<Edge> getEdges() const;

		
		// TODO get edges for one site function
		// TODO get next site in direction
		// TODO use std::reference_wrapper and compute with references... not to duplicate or round points


	private:
		/// List of all found edges
		std::list<Edge> _edges;

		/// Beachline or also "borderline". This is root of beachline tree.
		ParabolaNode _beachline;

		/// Bounding box is input paramter
		BoundingBox _boundingBox;

		/// Take high priority (big "y" coordinate) events first
		std::priority_queue<std::unique_ptr<VertexEvent>, std::vector<std::unique_ptr<VertexEvent>>> _vertexEventQueue;

		/// Queue of site events
		std::vector<SiteEvent> _siteEventQueue;

		void _generate();
		void _postprocessing();
		void _processEvent(const SiteEvent * event);
		void _processEvent(VertexEvent * event);
		void _circleEvent(ParabolaNode * parabola, const double sweepline);
	};
}


#endif  // VORONOI_H
